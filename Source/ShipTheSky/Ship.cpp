// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "MapManager.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "STSGameState.h"
#include "Tile/GuardianTile.h"
#include "Unit/BaseUnit.h"
#include "Battle/BattleComponent.h"

// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->CastShadow = false;

	Durability = 100;
	FlightPower = 3;
	UnitCapacity = 4;

	CloudResistance = FMath::RandRange(0, 10);
	StormResistance = FMath::RandRange(0, 10);
	SunResistance = FMath::RandRange(0, 10);
	LightningResistance = FMath::RandRange(0, 10);
	MeteorResistance = FMath::RandRange(0, 10);
	bIsBeingObserved = false;
}

void AShip::Tick(float DeltaTime)
{
	FVector LerpedPosition = FMath::Lerp(GetActorLocation(), CurTile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f), 0.05f);

	SetActorLocation(LerpedPosition);
}

bool AShip::TryLocateOnTile(ABaseTile* Tile, bool RightAfter)
{
	AIslandTile* IslandTile = Cast<AIslandTile>(Tile);
	if (IslandTile && IslandTile->GetIslandOwner() != OwnerCommander) return false;

	if (Tile == nullptr) return false;
	if (Tile->GetShip() != nullptr) return false;

	if (CurTile != nullptr) CurTile->SetShip(nullptr);

	if (RightAfter) SetActorLocation(Tile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f));
	CurTile = Tile;
	Tile->SetShip(this);
	SetActorHiddenInGame(false);
	return true;
}

void AShip::InitializeStatWithResources(int32 WoodCloud, int32 WoodStorm, int32 WoodSun, int32 WoodLightning, int32 WoodMeteor)
{
	Durability = 100;

	CloudResistance = WoodCloud;
	StormResistance = WoodStorm;
	SunResistance = WoodSun;
	LightningResistance = WoodLightning;
	MeteorResistance = WoodMeteor;

	FlightPower = 5;

	UnitCapacity = 4;
}

bool AShip::AddUnit(ABaseUnit* Unit)
{
	if (Units.Contains(Unit)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Already In"));
		return false;
	}
	if (Units.Num() == UnitCapacity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Full"));
		return false;
	}
	Units.Add(Unit);
	return true;
}

bool AShip::RemoveUnit(class ABaseUnit* Unit)
{
	if (!Units.Contains(Unit))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Exists"));
		return false;
	}
	Units.Remove(Unit);
	return true;
}

bool AShip::TryAddTileToPath(ABaseTile* Tile, bool bIsFirstPath)
{
	AIslandTile* IslandTile = Cast<AIslandTile>(Tile);
	if (IslandTile && IslandTile->GetIslandOwner() != OwnerCommander) return false;
	if (bIsFirstPath) //첫 클릭일때
	{
		UE_LOG(LogTemp, Warning, TEXT("CheckFirst"));
		if (Path.Num() == 0 && Tile != CurTile) return false; //진짜 첫 클릭인데 현위치 타일을 누른게 아님
		if (Path.Num() != 0 && Tile != Path.Last(0)) return false; //이어서 첫 클릭인데 마지막으로 드래그한 타일을 누른게 아님
		return true;
	}
	else if (Path.Num() != 0 && Path.Last(0) == Tile) //마지막 타일과 동일한 것임
	{
		return false;
	}
	else
	{
		if (Path.Num() != 0) //경로가 존재하는 상황에서 인접한 타일인지 확인
		{
			TArray<ABaseTile*> OutArray;
			GetGameInstance()->GetSubsystem<UMapManager>()->GetAdjacentTiles(Path.Last(0), OutArray);
			bool IsAdjacent = false;
			for (auto elem : OutArray)
			{
				if (elem == Tile)
				{
					IsAdjacent = true;
					break;
				}
			}
			if (!IsAdjacent) return false;
		}
		Path.Add(Tile);
		return true;
	}
}

void AShip::EmptyPath()
{
	if (bIsBeingObserved) ClearPathUI();
	Path.Empty();
}

void AShip::FollowPath()
{
	UE_LOG(LogTemp, Warning, TEXT("Next"));
	if (!Path.IsEmpty())
	{
		if (Path[0] == CurTile)
		{
			if (bIsBeingObserved) RemoveFrontPathUI();
			Path.RemoveAt(0);
			if (Path.IsEmpty()) return;
		}
		bool Success = TryLocateOnTile(Path[0], false);
		if (Success)
		{
			if (bIsBeingObserved) RemoveFrontPathUI();
			Path.RemoveAt(0);
		}

		if (!Path.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Next"));
			GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AShip::FollowPath, FlightPower, false, FlightPower);
		}
	}
}

void AShip::ClearPathUI()
{
	for (auto Tile : Path)
	{
		Tile->SetTileUI(0);
	}
	UE_LOG(LogTemp, Warning, TEXT("Cleared"));
}

void AShip::UpdatePathUI()
{
	int32 idx = 1;
	for (auto Tile : Path)
	{
		Tile->SetTileUI(idx++);
	}
}

void AShip::RemoveFrontPathUI()
{
	if (Path.IsEmpty()) return;
	Path[0]->SetTileUI(0);
}

void AShip::Teleport(ABaseTile* Tile)
{
	EmptyPath();
	TryLocateOnTile(Tile, true);
}

void AShip::GetAttacked(float Damage)
{
	if (!CanShipAttack())
	{
		DestroyShip();
	}
	else
	{
		check(Units.Num() != 0);
		ABaseUnit* Target = nullptr;
		//전사부터
		while (Damage != 0)
		{
			Target = nullptr;
			for (int32 Idx = Units.Num() - 1; Idx >= 0; Idx--)
			{
				ABaseUnit* Unit = Units[Idx];
				if (Unit->GetUnitType() == EUnitType::Warrior)
				{
					if (Target == nullptr || Target->BattleComponent->GetCurHP() < Unit->BattleComponent->GetCurHP())
					{
						Target = Unit;
					}
				}
			}
			if (Target)
			{
				Damage -= Target->GetAttacked(Damage);
				if (!Target->BattleComponent->IsAlive()) Target->DestroyUnit();
			}
			else break;
		}

		//HP부터
		while (Damage != 0 && !Units.IsEmpty())
		{
			Target = Units.Last();
			for (int32 Idx = Units.Num() - 2; Idx >= 0; Idx--)
			{
				ABaseUnit* Unit = Units[Idx];
				if (Target->BattleComponent->GetCurHP() < Unit->BattleComponent->GetCurHP())
				{
					Target = Unit;
				}
			}
			Damage -= Target->GetAttacked(Damage);
			if (!Target->BattleComponent->IsAlive()) Target->DestroyUnit();
		}
	}
}

float AShip::Attack()
{
	float Damage = 0.0f;
	for (auto Unit : Units) Damage += Unit->BattleComponent->GetDamage();
	return Damage;
}

void AShip::DestroyShip()
{
	CurTile->SetShip(nullptr);
	Destroy();
}
