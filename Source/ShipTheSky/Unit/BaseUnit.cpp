// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/BaseUnit.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "Tile/ResourceTile.h"
#include "Tile/GuardianTile.h"
#include "MapManager.h"
#include "Pawn/Commander.h"
#include "Ship.h"
#include "Animation/UnitAnimInstance.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent0"));
	SkeletalMeshComponent ->SetupAttachment(Root);
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMeshComponent->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	SkeletalMeshComponent->CastShadow = false;

	MoveSpeed = 500.0f;
}

void ABaseUnit::LocateOnIslandTile(AIslandTile* Tile, bool bIsImmediate)
{
	if (Tile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	if (Tile->GetIslandID() != CurIslandTile->GetIslandID())
	{
		UE_LOG(LogTemp, Error, TEXT("Different Island"));
		return;
	}

	AGuardianTile* MainTile = Tile->GetGuardianTile();
	if (MainTile == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Island"));
		return;
	}

	ABaseTile* PrevTile = CurIslandTile;
	if (Cast<AGuardianTile>(Tile) == MainTile)
	{
		CurIslandTile = Tile;
		if (bIsImmediate)
		{
			SetActorLocation(Tile->GetActorLocation());
			SetActorHiddenInGame(true);
		}
	}
	else
	{
		Cast<AResourceTile>(Tile)->SetUnit(this);
		CurIslandTile = Tile;
		if (bIsImmediate)
		{
			FVector Direction = MainTile->GetActorLocation() - Tile->GetActorLocation();
			SetActorRotation(Direction.Rotation());
			SetActorLocation(Tile->GetActorLocation());
			SetActorHiddenInGame(false);
		}
		else SetActorHiddenInGame(true);
	}
	AResourceTile* PrevResourceTile = Cast<AResourceTile>(PrevTile);
	if (PrevResourceTile && PrevResourceTile->GetUnit() == this) PrevResourceTile->SetUnit(nullptr);
	if (!bIsImmediate) MakePath(PrevTile, CurIslandTile);
}

bool ABaseUnit::Embark(AShip* Ship)
{
	if (Ship != nullptr && CurIslandTile != nullptr && Ship->AddUnit(this))
	{
		SetActorHiddenInGame(true);

		if (CurIslandTile != nullptr)
		{
			AResourceTile* Tile = Cast<AResourceTile>(CurIslandTile);
			if (Tile != nullptr) Tile->SetUnit(nullptr);
			GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(CurIslandTile->GetIslandID())->RemoveUnitFromThisIsland(this);
			SetCurIslandTile(nullptr);
		}

		CurShip = Ship;
		return true;
	}
	if (Ship == nullptr) UE_LOG(LogTemp, Warning, TEXT("Ship nullptr fail"));
	if (CurIslandTile == nullptr) UE_LOG(LogTemp, Warning, TEXT("CurTile nullptr fail"));
	if (!Ship->AddUnit(this)) UE_LOG(LogTemp, Warning, TEXT("Add Unit fail"));
	return false;
}

bool ABaseUnit::Disembark()
{
	if (CurShip != nullptr && CurShip->RemoveUnit(this))
	{
		AIslandTile* IslandTile = Cast<AIslandTile>(CurShip->GetCurTile());
		if (IslandTile == nullptr) return false;
		int32 IslandID = IslandTile->GetIslandID();
		AGuardianTile* GuardianTile = GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(IslandID);
		if (GuardianTile != nullptr)
		{
			CurShip = nullptr;
			GuardianTile->AddUnitOnThisIsland(this);
			Cast<ACommander>(GetOwner())->FillIslandWithUnit(IslandID, this);
			return true;
		}
	}
	return false;
}

void ABaseUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Path.IsEmpty()) return;
	if (FVector::Distance(Path[0]->GetActorLocation(), GetActorLocation()) > KINDA_SMALL_NUMBER)
	{
		FRotator Direction = (Path[0]->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(Direction);
		FVector Offset = GetActorForwardVector() * MoveSpeed * DeltaSeconds;
		
		if (FVector::Distance(Path[0]->GetActorLocation(), GetActorLocation()) < MoveSpeed * DeltaSeconds)
		{
			SetActorLocation(Path[0]->GetActorLocation());
		}
		else
		{
			SetActorLocation(GetActorLocation() + Offset);
			//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), Offset.X, Offset.Y, Offset.Z);
		}
	}
	else
	{
		if (Path.Num() == 1)
		{
			AResourceTile* ResourceTile = Cast<AResourceTile>(Path[0]);
			if (ResourceTile == nullptr)
			{
				SetActorHiddenInGame(true);
			}
			else
			{
				AGuardianTile* MainTile = ResourceTile->GetGuardianTile();
				FVector Direction = MainTile->GetActorLocation() - ResourceTile->GetActorLocation();
				SetActorLocation(ResourceTile->GetActorLocation());
				SetActorRotation(Direction.Rotation());
				SetActorHiddenInGame(false);
			}
			AnimInstance->bIsIdle = true;
			AnimInstance->bIsWalking = false;
		}
		Path.RemoveAt(0);
		UE_LOG(LogTemp, Warning, TEXT("RemovePath"));
	}
}

void ABaseUnit::MakePath(class ABaseTile* From, class ABaseTile* To)
{
	TArray<ABaseTile*> AdjTiles;
	GetGameInstance()->GetSubsystem<UMapManager>()->GetAdjacentTiles(From, AdjTiles);
	bool IsAdjacent;
	for (auto Tile : AdjTiles)
	{
		if (Tile == To)
		{
			IsAdjacent = true;
			break;
		}
	}
	if (!IsAdjacent)
	{
		Path.Add(Cast<AIslandTile>(From)->GetGuardianTile());
	}
	Path.Add(To);
	AnimInstance->bIsIdle = false;
	AnimInstance->bIsWalking = true;
	SetActorHiddenInGame(false);
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UUnitAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
}
