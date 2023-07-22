// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/GuardianTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"
#include "Guardian/Guardian.h"
#include "Ship.h"
#include "Battle/BattleComponent.h"
#include "Tile/ResourceTile.h"
#include "Tile/IslandTile.h"
#include "STSGameState.h"
#include "Building/BaseBuilding.h"
#include "Enums.h"
#include "STSGameState.h"

AGuardianTile::AGuardianTile()
{
	IslandType = EIslandTileType::Guardian;
	bIsAttackedRecently = false;
	RecoveryCount = 0;
	RepeatRecover = 0;
}

void AGuardianTile::AddUnitOnThisIsland(ABaseUnit* Unit)
{
	if (UnitsOnThisIsland.Contains(Unit)) return;
	else
	{
		if (Unit == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong Unit"));
		}
		else
		{
			UnitsOnThisIsland.Add(Unit);
			Unit->SetCurIslandTile(this);
		}
	}
}

void AGuardianTile::RemoveUnitFromThisIsland(ABaseUnit* Unit)
{
	if (UnitsOnThisIsland.Contains(Unit))
	{
		UnitsOnThisIsland.Remove(Unit);
		if (Unit->GetCurIslandTile() && Unit->GetCurIslandTile()->GetIslandType() != EIslandTileType::Guardian) Cast<AResourceTile>(Unit->GetCurIslandTile())->SetUnit(nullptr);
		Unit->SetCurIslandTile(nullptr);
	}
}

void AGuardianTile::TimePass(int32 GameDate)
{
	Super::TimePass(GameDate);

	if (CheckEnemyShipAdjacent())
	{
		RecoveryCount = 0;
		RepeatRecover = 0;
		AttackShips();
		GetAttackedByShips();
		bIsAttackedRecently = true;
	}
	else
	{
		if (RecoveryCount == 30)
		{
			RecoverFriendly();
			if (RepeatRecover < 10) RepeatRecover++;
			else bIsAttackedRecently = false;
		}
		else RecoveryCount++;
	}

	if (GameDate % 30 == 1)
	{
		ASTSGameState* GameState = GetWorld()->GetGameState<ASTSGameState>();
		ACommander* IslandOwner = GameState->GetIslandOwner(IslandID);
		if (IslandOwner) GameState->CommanderScores[IslandOwner->CommanderID] += Guardian->GetScorePower();
	}
}

void AGuardianTile::OptimizeUnitPlacementForResource()
{
	UE_LOG(LogTemp, Warning, TEXT("Resource"));
	ACommander* Commander = GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(IslandID);
	if (!Commander)
	{
		return;
	}
	TArray<ABaseUnit*> Miners;
	TArray<ABaseUnit*> Woodcutters;
	TArray<ABaseUnit*> Farmers;
	for (auto Unit : UnitsOnThisIsland)
	{
		switch (Unit->GetUnitType())
		{
		case EUnitType::Miner:
			Miners.Add(Unit);
			break;
		case EUnitType::Woodcutter:
			Woodcutters.Add(Unit);
			break;
		case EUnitType::Farmer:
			Farmers.Add(Unit);
			break;
		default:
			break;
		}
	}
	Miners.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
		return A.GetEfficiency() < B.GetEfficiency();
		});
	Woodcutters.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
		return A.GetEfficiency() < B.GetEfficiency();
		});
	Farmers.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
		return A.GetEfficiency() < B.GetEfficiency();
		});
	for (auto Tile : GetAdjResourceTiles())
	{
		switch (Tile->GetIslandType())
		{
		case EIslandTileType::Mine:
			if (!Miners.IsEmpty())
			{
				Commander->TryRelocateUnitOnTile(Miners.Last(), Tile);
				Miners.Pop();
			}
			break;
		case EIslandTileType::Forest:
			if (!Woodcutters.IsEmpty())
			{
				Commander->TryRelocateUnitOnTile(Woodcutters.Last(), Tile);
				Woodcutters.Pop();
			}
			break;
		case EIslandTileType::Farm:
			if (!Farmers.IsEmpty())
			{
				Commander->TryRelocateUnitOnTile(Farmers.Last(), Tile);
				Farmers.Pop();
			}
			break;
		default:
			break;
		}
	}
}

void AGuardianTile::OptimizeUnitPlacementForDefense()
{
	UE_LOG(LogTemp, Warning, TEXT("Defense"));
	ACommander* Commander = GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(IslandID);
	if (!Commander)
	{
		return;
	}
	TArray<ABaseUnit*> Warriors;
	TArray<ABaseUnit*> NotWarriors;
	for (auto Unit : UnitsOnThisIsland)
	{
		if (Unit->GetUnitType() == EUnitType::Warrior) Warriors.Add(Unit);
		else NotWarriors.Add(Unit);
		Warriors.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.BattleComponent->GetCurHP() < B.BattleComponent->GetCurHP();
			});
		NotWarriors.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.BattleComponent->GetCurHP() < B.BattleComponent->GetCurHP();
			});
	}
	for (auto Tile : GetAdjResourceTiles())
	{
		if (!Warriors.IsEmpty())
		{
			Commander->TryRelocateUnitOnTile(Warriors.Last(), Tile);
			Warriors.Pop();
		}
		else if (!NotWarriors.IsEmpty())
		{
			Commander->TryRelocateUnitOnTile(NotWarriors.Last(), Tile);
			NotWarriors.Pop();
		}
		else break;
	}
}

void AGuardianTile::OptimizeUnitPlacementForRecovery()
{
	UE_LOG(LogTemp, Warning, TEXT("Recovery"));
	ACommander* Commander = GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(IslandID);
	if (!Commander)
	{
		return;
	}
	
	TArray<ABaseUnit*> Hurts;
	for (auto Unit : UnitsOnThisIsland)
	{
		if (Unit->BattleComponent->GetCurHP() != Unit->BattleComponent->GetMaxHP()) Hurts.Add(Unit);
	}
	for (auto Tile : GetAdjResourceTiles())
	{
		if (!Hurts.IsEmpty())
		{
			Commander->TryRelocateUnitOnTile(Hurts.Last(), Tile);
			Hurts.Pop();
		}
	}
}

void AGuardianTile::OptimizeUnitPlacementForBest()
{
	ACommander* Commander = GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(IslandID);
	if (!Commander)
	{
		return;
	}
	if (bIsAttackedRecently) OptimizeUnitPlacementForDefense();
	else
	{

		TArray<ABaseUnit*> Hurts;
		TArray<ABaseUnit*> Miners;
		TArray<ABaseUnit*> Woodcutters;
		TArray<ABaseUnit*> Farmers;
		TArray<ABaseUnit*> Warriors;
		for (auto Unit : UnitsOnThisIsland)
		{
			if (Unit->BattleComponent->GetCurHP() != Unit->BattleComponent->GetMaxHP()) Hurts.Add(Unit);
			else
			{
				switch (Unit->GetUnitType())
				{
				case EUnitType::Miner:
					Miners.Add(Unit);
					break;
				case EUnitType::Woodcutter:
					Woodcutters.Add(Unit);
					break;
				case EUnitType::Farmer:
					Farmers.Add(Unit);
					break;
				case EUnitType::Warrior:
					Warriors.Add(Unit);
				default:
					break;
				}
			}
		}

		Miners.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.GetEfficiency() < B.GetEfficiency();
			});
		Woodcutters.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.GetEfficiency() < B.GetEfficiency();
			});
		Farmers.Sort([](const ABaseUnit& A, const ABaseUnit& B) {
			return A.GetEfficiency() < B.GetEfficiency();
			});
		for (auto Tile : GetAdjResourceTiles())
		{
			if (!Hurts.IsEmpty())
			{
				Commander->TryRelocateUnitOnTile(Hurts.Last(), Tile);
				Hurts.Pop();
			}
			else
			{
				switch (Tile->GetIslandType())
				{
				case EIslandTileType::Mine:
					if (!Miners.IsEmpty())
					{
						Commander->TryRelocateUnitOnTile(Miners.Last(), Tile);
						Miners.Pop();
					}
					break;
				case EIslandTileType::Forest:
					if (!Woodcutters.IsEmpty())
					{
						Commander->TryRelocateUnitOnTile(Woodcutters.Last(), Tile);
						Woodcutters.Pop();
					}
					break;
				case EIslandTileType::Farm:
					if (!Farmers.IsEmpty())
					{
						Commander->TryRelocateUnitOnTile(Farmers.Last(), Tile);
						Farmers.Pop();
					}
					break;
				default:
					break;
				}
			}
		}

		/*for (auto Unit : Hurts) Commander->TryRelocateUnitOnTile(Unit, this);
		for (auto Unit : Miners) Commander->TryRelocateUnitOnTile(Unit, this);
		for (auto Unit : Woodcutters) Commander->TryRelocateUnitOnTile(Unit, this);
		for (auto Unit : Farmers) Commander->TryRelocateUnitOnTile(Unit, this);
		for (auto Unit : Warriors) Commander->TryRelocateUnitOnTile(Unit, this);*/
	}
}

void AGuardianTile::SpawnGuardian(int32 Index)
{
	if (Guardian) return;
	FTransform Loc(GetActorLocation());
	Guardian = GetWorld()->SpawnActorDeferred<AGuardian>(Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController())->GuardianClass[Index], Loc);
	Guardian->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	Guardian->Tile = this;
	Guardian->FinishSpawning(Loc);
}

bool AGuardianTile::CheckEnemyShipAdjacent()
{
	EnemyShips.Empty();
	for (auto AdjTile : AdjTiles)
	{
		if (AdjTile->GetShip() && AdjTile->GetShip()->GetOwnerCommander() != GetIslandOwner())
		{
			EnemyShips.Add(AdjTile->GetShip());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
		}
	}
	return EnemyShips.Num() != 0;
}

void AGuardianTile::AttackShips()
{
	float Damage = Guardian->BattleComponent->GetDamage();
	for (auto Tile : AdjResourceTiles)
	{
		if (Tile->GetUnit())
		{
			Damage += Tile->GetUnit()->BattleComponent->GetDamage();
		}
	}
	Damage /= EnemyShips.Num();
	for (int32 Idx = EnemyShips.Num() - 1; Idx >= 0; Idx--)
	{
		EnemyShips[Idx]->GetAttacked(Damage);
	}
}

void AGuardianTile::GetAttackedByShips()
{
	//************여기 소유권 바뀌면 타이머 취소되도록 해줘야 해***********//
	ABaseUnit* Target = nullptr;

	float Damage = 0.0f;
	for (auto Ship : EnemyShips)
	{
		if (Ship && Ship->CanShipAttack()) Damage += Ship->Attack();
	}

	//전사부터
	while (Damage != 0)
	{
		Target = nullptr;
		for (int32 Idx = AdjResourceTiles.Num() - 1; Idx >= 0; Idx--)
		{
			ABaseUnit* Unit = AdjResourceTiles[Idx]->GetUnit();
			if (Unit && Unit->GetUnitType() == EUnitType::Warrior)
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
				GetIslandOwner()->DestroyUnitFromGame(Target);
			}
		}
		else break;
	}

	//HP부터
	while (Damage != 0)
	{
		Target = nullptr;
		for (int32 Idx = AdjResourceTiles.Num() - 1; Idx >= 0; Idx--)
		{
			ABaseUnit* Unit = AdjResourceTiles[Idx]->GetUnit();
			if (Unit)
			{
				if (!Target || Target->BattleComponent->GetCurHP() < Unit->BattleComponent->GetCurHP()) Target = Unit;
			}
		}
		if (Target)
		{
			Damage -= Target->GetAttacked(Damage);
			if (!Target->BattleComponent->IsAlive())
			{
				GetIslandOwner()->DestroyUnitFromGame(Target);
			}
		}
		else break;
	}
	
	Damage -= Guardian->BattleComponent->TakeDamage(Damage);
	if (Damage != 0)
	{
		for (auto Tile : AdjResourceTiles)
		{
			if (Tile->GetShip()) Tile->GetShip()->GetAttacked(9999999.0f);
			if (Tile->GetBuilding()) GetIslandOwner()->DestroyBuildingFromGame(Tile->GetBuilding());
		}
		for (int32 Idx = UnitsOnThisIsland.Num() - 1; Idx >= 0; Idx--)
		{
			if (!UnitsOnThisIsland.IsValidIndex(Idx) || !UnitsOnThisIsland[Idx])
			{
				UE_LOG(LogTemp, Error, TEXT("Destroying Unit Failed... %s"), *GetName());
			}
			else UnitsOnThisIsland[Idx]->GetOwnerCommander()->DestroyUnitFromGame(UnitsOnThisIsland[Idx]);
		}
		GetWorld()->GetGameState<ASTSGameState>()->SetIslandOwner(GetIslandID(), GetNewOwner());
		Guardian->ResetLevelAndPower();
		SetOutline();
		for (auto Tile : AdjResourceTiles) Tile->SetOutline();
	}
}

ACommander* AGuardianTile::GetNewOwner()
{
	TMap<ACommander*, int32> Count;
	int32 MaxCount = 0;
	TArray<ACommander*> ReturnCommander;
	for (auto Ship : EnemyShips)
	{
		if (Ship)
		{
			if (Count.Contains(Ship->GetOwnerCommander())) Count[Ship->GetOwnerCommander()]++;
			else Count.Add(Ship->GetOwnerCommander(), 0);
			if (MaxCount < Count[Ship->GetOwnerCommander()])
			{
				ReturnCommander.Empty();
				ReturnCommander.Add(Ship->GetOwnerCommander());
				MaxCount = Count[Ship->GetOwnerCommander()];
			}
			else if (MaxCount == Count[Ship->GetOwnerCommander()])
			{
				ReturnCommander.Add(Ship->GetOwnerCommander());
			}
		}
	}
	if (ReturnCommander.IsEmpty()) return nullptr;
	else return ReturnCommander[FMath::RandRange(0, ReturnCommander.Num() - 1)];
}

int32 AGuardianTile::GetBattlePower() const
{
	if (!Guardian) return -1;
	int32 TotalPower = 0;
	for (auto Unit : UnitsOnThisIsland)
	{
		TotalPower += Unit->GetBattlePower();
	}
	TotalPower += Guardian->GetBattlePower();
	return TotalPower;
}

void AGuardianTile::RecoverFriendly()
{
	for (auto Tile : AdjResourceTiles)
	{
		if (Tile->GetUnit())
		{
			Tile->GetUnit()->BattleComponent->RecoverHP(Tile->GetUnit()->BattleComponent->GetMaxHP() * 0.1f);
		}	
		if (Tile->GetShip())
		{
			Tile->GetShip()->RecoverDurability(Tile->GetShip()->GetMaxDurability() * 0.05f);
		}
	}
	Guardian->BattleComponent->RecoverHP(Guardian->BattleComponent->GetMaxHP() * 0.05f);
}

void AGuardianTile::SetAdjTiles(TArray<class ABaseTile*>& InTile)
{
	AdjTiles.Empty();
	for (auto Elem : InTile) AdjTiles.Add(Elem);
}

void AGuardianTile::SetAdjResourceTiles(TArray<class AResourceTile*>& InTile)
{
	AdjResourceTiles.Empty();
	for (auto Elem : InTile) AdjResourceTiles.Add(Elem);
	AdjResourceTiles.Sort([](const AResourceTile& A, const AResourceTile& B) {
		return A.TotalResourcesNum > B.TotalResourcesNum;
		});
}

bool AGuardianTile::IsBuildingTypeBuilt(EBuildingType Type)
{
	for (auto Tile : AdjResourceTiles)
	{
		if (Tile->GetBuilding() && Tile->GetBuilding()->GetBuildingType() == Type) return true;
	}

	return false;
}

void AGuardianTile::BeginPlay()
{
	Super::BeginPlay();
}