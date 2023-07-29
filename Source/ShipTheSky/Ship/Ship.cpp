// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship/Ship.h"
#include "Tile/BaseTile.h"
#include "Tile/IslandTile.h"
#include "Game/MapManager.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Game/STSGameState.h"
#include "Tile/GuardianTile.h"
#include "Unit/BaseUnit.h"
#include "Building/BaseBuilding.h"
#include "Tile/ResourceTile.h"
#include "Battle/BattleComponent.h"

// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->CastShadow = false;

	MaxDurability = 100;
	CurDurability = 100;
	OriginalFlightPower = 5;
	ModifiedFlightPower = OriginalFlightPower;
	UnitCapacity = 4;

	CloudResistance = 0;
	StormResistance = 0;
	SunResistance = 0;
	LightningResistance = 0;
	MeteorResistance = 0;
	bIsBeingObserved = false;


	DurabilityStormLevel = 1;
	DurabilityLightningLevel = 1;
	FlightPowerCloudLevel = 1;
	FlightPowerSunLevel = 1;
	CloudLevel = 1;
	StormLevel = 1;
	SunLevel = 1;
	LightningLevel = 1;
	MeteorLevel = 1;

	ResistanceDelta = 5;
	DurabilityDelta = 10;
	FlightPowerDelta = 0.95f;

	ShipStatus = EShipStatus::None;
	StuckCount = 0;
}

void AShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector LerpedPosition = FMath::Lerp(GetActorLocation(), CurTile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f), 0.05f);

	SetActorLocation(LerpedPosition);

	if (bIsAttackedRecently)
	{
		LastAttackedSecond += DeltaSeconds;
		if (LastAttackedSecond >= 6.0f)
		{
			bIsAttackedRecently = false;
			ShipStatus = EShipStatus::None;
			BattlePower = 0;
			AttackTargetGuardianTile = nullptr;
		}
	}
	else LastAttackedSecond = 0.0f;
}

bool AShip::TryLocateOnTile(ABaseTile* Tile, bool RightAfter)
{
	AIslandTile* IslandTile = Cast<AIslandTile>(Tile);
	if (IslandTile && IslandTile->GetIslandOwner() != OwnerCommander) return false;

	if (Tile == nullptr) return false;
	if (Tile->GetShip() != nullptr)
	{
		return false;
	}

	if (CurTile != nullptr)
	{
		FVector Direction = Tile->GetActorLocation() - CurTile->GetActorLocation();
		SetActorRotation(Direction.Rotation());
		CurTile->SetShip(nullptr);
	}



	if (RightAfter) SetActorLocation(Tile->GetActorLocation() + FVector(0.0f, 0.0f, 250.0f));
	else SetCurDurability(CurDurability - 1);

	CurTile = Tile;
	Tile->SetShip(this);
	SetActorHiddenInGame(false);
	return true;
}

void AShip::SetMaxDurability(float NewDurability)
{
	MaxDurability = NewDurability;
}

void AShip::SetCurDurability(float NewDurability)
{
	CurDurability = FMath::Clamp(NewDurability, 0, MaxDurability);
	if (CurDurability == 0)
	{
		SetModifiedFlightPower(GetOriginalFlightPower() * 2);
	}
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
	if (IslandTile && IslandTile->GetIslandType() == EIslandTileType::Guardian) return false;
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
	GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
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
			StuckCount = 0;
		}
		else
		{
			StuckCount++;
			if (ShipStatus != EShipStatus::None && StuckCount == 10)
			{
				EmptyPath();
				SetShipStatus(EShipStatus::None);
			}
		}

		if (!Path.IsEmpty())
		{
			GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AShip::FollowPath, ModifiedFlightPower, false, ModifiedFlightPower);

			if (ShipStatus == EShipStatus::MoveForBattle)
			{
				if (AttackTargetGuardianTile && BattlePower < AttackTargetGuardianTile->GetBattlePower())
				{
					EmptyPath();
					SetShipStatus(EShipStatus::None);
				}
			}
		}
		else
		{
			TArray<ABaseTile*> CheckBattleArr;
			AGuardianTile* CheckBattleGuardian;
			UMapManager* Manager = GetGameInstance()->GetSubsystem<UMapManager>();
			switch (ShipStatus)
			{
			case EShipStatus::MoveForDurability:
				DisembarkAllUnits();
				ShipStatus = EShipStatus::None;
				break;
			case EShipStatus::MoveForBattle:
				if (CurTile)
				{
					Manager->GetAdjacentTiles(CurTile, CheckBattleArr);
					for (auto adj : CheckBattleArr)
					{
						if (adj->GetTileType() == ETileType::Island)
						{
							CheckBattleGuardian = Cast<AGuardianTile>(adj);
							if (CheckBattleGuardian && CheckBattleGuardian->GetIslandOwner() != OwnerCommander)
							{
								ShipStatus = EShipStatus::InBattle;
								break;
							}
						}
					}
				}
				if (ShipStatus != EShipStatus::InBattle) ShipStatus = EShipStatus::None;
				break;
			default:
				ShipStatus = EShipStatus::None;
				break;
			}
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

FString AShip::GetStatusString()
{
	if (bIsAttackedRecently) return FString(TEXT("전투 중!"));
	if (Path.Num() != 0) return FString(TEXT("이동 중..."));
	if (!CurTile)
	{
		UE_LOG(LogTemp, Error, TEXT("CurTile must not be nullptr"));
		return FString();
	}
	switch (CurTile->GetTileType())
	{
	case ETileType::Island:
		return FString(TEXT("섬에 정박함"));
	case ETileType::Cloud:
		return FString(TEXT("먹구름 지역에 정지함"));
	case ETileType::Storm:
		return FString(TEXT("폭풍우 지역에 정지함"));
	case ETileType::Sun:
		return FString(TEXT("작열 지역에 정지함"));
	case ETileType::Lightning:
		return FString(TEXT("뇌운 지역에 정지함"));
	case ETileType::Meteor:
		return FString(TEXT("운석 지역에 정지함"));
	default:
		return FString(TEXT("상태 메시지"));
	}
}

void AShip::RemoveAllUnitsFromGame()
{
	for (int32 Idx = Units.Num() - 1; Idx >= 0; Idx--)
	{
		OwnerCommander->DestroyUnitFromGame(Units[Idx]);
	}
}

void AShip::ConsumeFoodForUnits(float Multiply)
{
	for (auto Unit : Units)
	{
		OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::Food) - Unit->GetFoodConsume() * Multiply, EResourceType::Food);
	}
}

void AShip::DecreaseHPForUnits(float Amount)
{
	for (auto Unit : Units)
	{
		if (Unit->BattleComponent->GetCurHP() > Amount + 1.0f) Unit->BattleComponent->TakeDamage(Amount);
	}
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
		OwnerCommander->DestroyShipFromGame(this);
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
				if (!Target->BattleComponent->IsAlive())
				{
					check(Target->GetOwnerCommander() == OwnerCommander);
					Target->GetOwnerCommander()->DestroyUnitFromGame(Target);
				}
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
			if (!Target->BattleComponent->IsAlive())
			{
				check(Target->GetOwnerCommander() == OwnerCommander);
				Target->GetOwnerCommander()->DestroyUnitFromGame(Target);
			}
		}
		bIsAttackedRecently = true;
		LastAttackedSecond = 0.0f;
		ShipStatus = EShipStatus::InBattle;
	}
}

float AShip::Attack()
{
	float Damage = 0.0f;
	for (auto Unit : Units) Damage += Unit->BattleComponent->GetDamage();
	return Damage;
}
 
 bool AShip::UpgradeResistance(enum ETileType Type)
{
	 AResourceTile* CurResourceTile = Cast<AResourceTile>(CurTile);
	 if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Shipyard) return false;
	 switch (Type)
	 {
	 case ETileType::Cloud:
		 if (OwnerCommander->GetResource(EResourceType::WoodCloud) < GetResistanceUpgradeCost(Type)) return false;
		 OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodCloud) - GetResistanceUpgradeCost(Type), EResourceType::WoodCloud);
		 CloudLevel++;
		 CloudResistance += ResistanceDelta;
		 return true;
	 case ETileType::Storm:
		 if (OwnerCommander->GetResource(EResourceType::WoodStorm) < GetResistanceUpgradeCost(Type)) return false;
		 OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodStorm) - GetResistanceUpgradeCost(Type), EResourceType::WoodStorm);
		 StormLevel++;
		 StormResistance += ResistanceDelta;
		 return true;
	 case ETileType::Sun:
		 if (OwnerCommander->GetResource(EResourceType::WoodSun) < GetResistanceUpgradeCost(Type)) return false;
		 OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodSun) - GetResistanceUpgradeCost(Type), EResourceType::WoodSun);
		 SunLevel++;
		 SunResistance += ResistanceDelta;
		 return true;
	 case ETileType::Lightning:
		 if (OwnerCommander->GetResource(EResourceType::WoodLightning) < GetResistanceUpgradeCost(Type)) return false;
		 OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodLightning) - GetResistanceUpgradeCost(Type), EResourceType::WoodLightning);
		 LightningLevel++;
		 LightningResistance += ResistanceDelta;
		 return true;
	 case ETileType::Meteor:
		 if (OwnerCommander->GetResource(EResourceType::WoodMeteor) < GetResistanceUpgradeCost(Type)) return false;
		 OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodMeteor) - GetResistanceUpgradeCost(Type), EResourceType::WoodMeteor);
		 MeteorLevel++;
		 MeteorResistance += ResistanceDelta;
		 return true;
	 }
	 return false;
}

int32 AShip::GetResistanceUpgradeCost(enum ETileType Type) const
{
	switch (Type)
	{
	case ETileType::Cloud:
		return CloudLevel * 10;
		break;
	case ETileType::Storm:
		return StormLevel * 10;
		break;
	case ETileType::Sun:
		return SunLevel * 10;
		break;
	case ETileType::Lightning:
		return LightningLevel * 10;
		break;
	case ETileType::Meteor:
		return MeteorLevel * 10;
	default:
		return 0;
	}
}

bool AShip::UpgradeFlightPowerWithCloud()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Shipyard) return false;
	if (OwnerCommander->GetResource(EResourceType::WoodCloud) < GetFlightPowerUpgradeCostWithCloud()) return false;
	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodCloud) - GetFlightPowerUpgradeCostWithCloud(), EResourceType::WoodCloud);
	FlightPowerCloudLevel++;
	SetOriginalFlightPower(OriginalFlightPower * FlightPowerDelta);
	return true;
}

bool AShip::UpgradeFlightPowerWithSun()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Shipyard) return false;
	if (OwnerCommander->GetResource(EResourceType::WoodSun) < GetFlightPowerUpgradeCostWithSun()) return false;
	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodSun) - GetFlightPowerUpgradeCostWithSun(), EResourceType::WoodSun);
	FlightPowerSunLevel++;
	SetOriginalFlightPower(OriginalFlightPower * FlightPowerDelta);
	return true;
}

bool AShip::UpgradeDurabilityWithStorm()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Shipyard) return false;
	if (OwnerCommander->GetResource(EResourceType::WoodStorm) < GetDurabilityUpgradeCostWithStorm()) return false;
	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodStorm) - GetDurabilityUpgradeCostWithStorm(), EResourceType::WoodStorm);
	DurabilityStormLevel++;
	MaxDurability += DurabilityDelta;
	return true;
}

bool AShip::UpgradeDurabilityWithLightning()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Shipyard) return false;
	if (OwnerCommander->GetResource(EResourceType::WoodLightning) < GetDurabilityUpgradeCostWithLightning()) return false;
	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodLightning) - GetDurabilityUpgradeCostWithLightning(), EResourceType::WoodLightning);
	DurabilityLightningLevel++;
	MaxDurability += DurabilityDelta;
	return true;
}

bool AShip::UpgradeCapacity()
{
	AResourceTile* CurResourceTile = Cast<AResourceTile>(CurTile);
	if (!CurResourceTile || !CurResourceTile->GetBuilding() || CurResourceTile->GetBuilding()->GetBuildingType() != EBuildingType::Shipyard) return false;
	if (UnitCapacity == 6) return false;
	if (OwnerCommander->GetResource(EResourceType::WoodMeteor) < GetDurabilityUpgradeCostWithLightning()) return false;
	OwnerCommander->SetResource(OwnerCommander->GetResource(EResourceType::WoodMeteor) - GetDurabilityUpgradeCostWithLightning(), EResourceType::WoodMeteor);
	UnitCapacity++;
	return true;
}

void AShip::RecoverDurability(float Amount)
{
	SetCurDurability(GetCurDurability() + Amount);
}

int32 AShip::GetCapacityUpgradeCost() const
{
	if (UnitCapacity == 6) return 0;
	return (UnitCapacity - 3) * 100;
}

TPair<EShipStat, int32> AShip::GetStatUpgradeRecommendation()
{
	EResourceType Wood = EResourceType::None;
	int32 MaxResource = -1;

	for (EResourceType Type = EResourceType::WoodCloud; Type != EResourceType::Food;)
	{
		if (OwnerCommander->GetResource(Type) > MaxResource)
		{
			Wood = Type;
			MaxResource = OwnerCommander->GetResource(Type);
		}
		Type = StaticCast<EResourceType>((uint8)Type + 1);
	}

	TPair<EShipStat, int32> Recommendation = TPair<EShipStat, int32>(EShipStat::None, 1987654321);
	int32 ResourceAmount = 0;

	switch (Wood)
	{
	case EResourceType::WoodCloud:
		ResourceAmount = GetFlightPowerUpgradeCostWithCloud();
		if (OwnerCommander->GetResource(Wood) > ResourceAmount) Recommendation =  TPair<EShipStat, int32>(EShipStat::FlightPowerCloud, ResourceAmount);
		break;
	case EResourceType::WoodStorm:
		ResourceAmount = GetDurabilityUpgradeCostWithStorm();
		if (OwnerCommander->GetResource(Wood) > ResourceAmount) Recommendation = TPair<EShipStat, int32>(EShipStat::DurabilityStorm, ResourceAmount);
		break;
	case EResourceType::WoodSun:
		ResourceAmount = GetFlightPowerUpgradeCostWithSun();
		if (OwnerCommander->GetResource(Wood) > ResourceAmount) Recommendation = TPair<EShipStat, int32>(EShipStat::FlightPowerSun, ResourceAmount);
		break;
	case EResourceType::WoodLightning:
		ResourceAmount = GetDurabilityUpgradeCostWithLightning();
		if (OwnerCommander->GetResource(Wood) > ResourceAmount) Recommendation = TPair<EShipStat, int32>(EShipStat::DurabilityLightning, ResourceAmount);
		break;
	case EResourceType::WoodMeteor:
		ResourceAmount = GetCapacityUpgradeCost();
		if (UnitCapacity != 6 && OwnerCommander->GetResource(Wood) > ResourceAmount) Recommendation = TPair<EShipStat, int32>(EShipStat::Capacity, ResourceAmount);
		break;
	default: Recommendation = TPair<EShipStat, int32>(EShipStat::None, 1987654321);
	}

	ResourceAmount = GetResistanceUpgradeCost(StaticCast<ETileType>((uint8)Wood - 4));
	if (OwnerCommander->GetResource(Wood) > ResourceAmount && ResourceAmount < Recommendation.Value)
		Recommendation = TPair<EShipStat, int32>(StaticCast<EShipStat>((uint8)Wood - 5), GetResistanceUpgradeCost(StaticCast<ETileType>((uint8)Wood - 4)));

	if (Recommendation.Key == EShipStat::None) Recommendation = TPair<EShipStat, int32>(EShipStat::None, 0);

	return Recommendation;
}

bool AShip::AddCandidateBattleUnit(ABaseUnit* Unit)
{
	if (CandidateBattleUnits.Num() < UnitCapacity)
	{
		CandidateBattleUnits.Add(Unit);
		return true;
	}
	return false;
}

void AShip::EmptyCandidateBattleUnit()
{
	CandidateBattleUnits.Empty();
}

bool AShip::CheckAlreadyCandidateBattleUnit(ABaseUnit* Unit)
{
	return CandidateBattleUnits.Contains(Unit);
}

bool AShip::EmbarkCandiateBattleUnits()
{
	if (CandidateBattleUnits.Num() == 0) return false;

	DisembarkAllUnits();

	for (int32 Idx = CandidateBattleUnits.Num() - 1; Idx >= 0; Idx--)
	{
		if (!CandidateBattleUnits.IsValidIndex(Idx) || !CandidateBattleUnits[Idx])
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Candidate Unit"));
			return false;
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CandidateBattleUnits[Idx]->GetName());
		if (CandidateBattleUnits.IsValidIndex(Idx) && CandidateBattleUnits[Idx])
		{
			if (!CandidateBattleUnits[Idx]->Embark(this))
			{
				UE_LOG(LogTemp, Warning, TEXT("Embark Failed!!!!"));
				return false;
			}
		}
		else return false;
	}
	EmptyCandidateBattleUnit();
	return true;
}

void AShip::DisembarkAllUnits()
{
	for (int32 Idx = Units.Num() - 1; Idx >= 0; Idx--)
	{
		if (Units.IsValidIndex(Idx) && Units[Idx]) Units[Idx]->Disembark();
	}
}
