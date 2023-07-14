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
#include "Battle/BattleComponent.h"
#include "Building/BaseBuilding.h"

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

	BattleComponent = CreateDefaultSubobject<UBattleComponent>(TEXT("BattleComponent"));

	HPLevel = 1;
	FoodConsumeLevel = 1;
	AttackLevel = 1;
	EfficiencyLevel = 1;
	AllUpLevel = 1;
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
			CurIslandTile->GetGuardianTile()->RemoveUnitFromThisIsland(this);
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
			OwnerCommander->FillIslandWithUnit(IslandID, this);
			return true;
		}
	}
	return false;
}

bool ABaseUnit::UpgradeHP()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurIslandTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Barracks) return false;
	if (OwnerCommander->GetResource(EResourceType::StoneCloud) < GetHPUpgradeCost()) return false;

	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::StoneCloud) - GetHPUpgradeCost(), EResourceType::StoneCloud);
	HPLevel++;
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP() + 10, false);
	return true;
}

bool ABaseUnit::UpgradeFoodConsume()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurIslandTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Barracks) return false;
	if (OwnerCommander->GetResource(EResourceType::StoneSun) < GetFoodConsumeUpgradeCost()) return false;

	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::StoneSun) - GetFoodConsumeUpgradeCost(), EResourceType::StoneSun);
	FoodConsumeLevel++;
	FoodConsume *= 0.95f;
	return true;
}

bool ABaseUnit::UpgradeAttack()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurIslandTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Barracks) return false;
	if (OwnerCommander->GetResource(EResourceType::StoneStorm) < GetAttackUpgradeCost()) return false;

	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::StoneStorm) - GetAttackUpgradeCost(), EResourceType::StoneStorm);
	AttackLevel++;
	BattleComponent->SetDamage(BattleComponent->GetDamage() + 1);
	return true;
}

bool ABaseUnit::UpgradeEfficiency()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurIslandTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Barracks) return false;
	if (OwnerCommander->GetResource(EResourceType::StoneLightning) < GetEfficiencyUpgradeCost()) return false;

	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::StoneLightning) - GetEfficiencyUpgradeCost(), EResourceType::StoneLightning);
	EfficiencyLevel++;
	Efficiency += 1.0f;
	return true;
}

bool ABaseUnit::UpgradeAll()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurIslandTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Barracks) return false;
	if (OwnerCommander->GetResource(EResourceType::StoneMeteor) < GetAllUpgradeCost()) return false;

	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::StoneMeteor) - GetAllUpgradeCost(), EResourceType::StoneMeteor);
	AllUpLevel++;
	BattleComponent->SetDamage(BattleComponent->GetDamage() + 0.5f);
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP() + 5, false);
	Efficiency += 0.5f;
	FoodConsume *= 0.975f;
	return true;
}

TPair<enum EUnitStat, int32> ABaseUnit::GetStatUpgradeRecommendation()
{
	EResourceType Stone = EResourceType::None;
	int32 MaxResource = -1;

	for (EResourceType Type = EResourceType::StoneCloud; Type != EResourceType::WoodCloud;)
	{
		if (OwnerCommander->GetResource(Type) > MaxResource)
		{
			Stone = Type;
			MaxResource = OwnerCommander->GetResource(Type);
		}
		Type = StaticCast<EResourceType>((uint8)Type + 1);
	}

	switch (Stone)
	{
	case EResourceType::StoneCloud: 
		if (OwnerCommander->GetResource(EResourceType::StoneCloud) >= GetHPUpgradeCost()) return TPair<enum EUnitStat, int32>(EUnitStat::HP, GetHPUpgradeCost());
		else return TPair<enum EUnitStat, int32>(EUnitStat::None, 0);
	case EResourceType::StoneStorm:
		if (OwnerCommander->GetResource(EResourceType::StoneStorm) >= GetAttackUpgradeCost()) return TPair<enum EUnitStat, int32>(EUnitStat::Attack, GetAttackUpgradeCost());
		else return TPair<enum EUnitStat, int32>(EUnitStat::None, 0);
	case EResourceType::StoneSun:
		if (OwnerCommander->GetResource(EResourceType::StoneSun) >= GetFoodConsumeUpgradeCost()) return TPair<enum EUnitStat, int32>(EUnitStat::FoodConsume, GetFoodConsumeUpgradeCost());
		else return TPair<enum EUnitStat, int32>(EUnitStat::None, 0);
	case EResourceType::StoneLightning:
		if (OwnerCommander->GetResource(EResourceType::StoneLightning) >= GetEfficiencyUpgradeCost()) return TPair<enum EUnitStat, int32>(EUnitStat::Efficiency, GetEfficiencyUpgradeCost());
		else return TPair<enum EUnitStat, int32>(EUnitStat::None, 0);
	case EResourceType::StoneMeteor:
		if (OwnerCommander->GetResource(EResourceType::StoneMeteor) >= GetAllUpgradeCost()) return TPair<enum EUnitStat, int32>(EUnitStat::All, GetAllUpgradeCost());
		else return TPair<enum EUnitStat, int32>(EUnitStat::None, 0);
	default: return TPair<enum EUnitStat, int32>(EUnitStat::None, 0);
	}
}

float ABaseUnit::GetAttacked(float Damage)
{
	if (UnitType == EUnitType::Warrior) Damage = Damage * 100.0f / (100.0f + Efficiency);
	return BattleComponent->TakeDamage(Damage);
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
