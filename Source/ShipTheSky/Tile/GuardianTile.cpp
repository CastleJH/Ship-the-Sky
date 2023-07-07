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

AGuardianTile::AGuardianTile()
{
	IslandType = EIslandTileType::Guardian;
}

void AGuardianTile::OnTileSelectedAsView(class ASTSPlayerController* PlayerController)
{
	Super::OnTileSelectedAsView(PlayerController);
	if (PlayerController->GetIsPathSelectionMode())
	{

	}
	else
	{
		if (PlayerController->GetCommander()->GetTargetIslandTile() == nullptr) return;
	}
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
		Unit->SetCurIslandTile(nullptr);
	}
}

void AGuardianTile::TimePass(int32 GameDate)
{
	if (CheckEnemyShipAdjacent())
	{
		RecoveryCount = 0;
		AttackShips();
		GetAttackedByShips();
	}
	else
	{
		if (RecoveryCount == 30) RecoverFriendly();
		else RecoveryCount++;
	}
}

void AGuardianTile::SpawnGuardian(int32 Index)
{
	FTransform Loc(GetActorLocation());
	Guardian = GetWorld()->SpawnActorDeferred<AGuardian>(Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController())->GuardianClass[Index], Loc);
	Guardian->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
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
				Target->DestroyUnit();
				TryFillIsland();
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
			if (Unit && Target->BattleComponent->GetCurHP() < Unit->BattleComponent->GetCurHP())
			{
				Target = Unit;
			}
		}
		if (Target)
		{
			Damage -= Target->GetAttacked(Damage);
			if (!Target->BattleComponent->IsAlive())
			{
				Target->DestroyUnit();
				TryFillIsland();
			}
		}
		else break;
	}

	Damage -= Guardian->BattleComponent->TakeDamage(Damage);
	if (Damage != 0)
	{
		GetWorld()->GetGameState<ASTSGameState>()->SetIslandOwner(GetIslandID(), GetNewOwner());
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

void AGuardianTile::RecoverFriendly()
{
	for (auto Tile : AdjResourceTiles)
	{
		if (Tile->GetUnit())
		{
			Tile->GetUnit()->BattleComponent->RecoverHP(Tile->GetUnit()->BattleComponent->GetMaxHP() * 0.1f);
		}			
	}
	Guardian->BattleComponent->RecoverHP(Guardian->BattleComponent->GetMaxHP() * 0.05f);
}

bool AGuardianTile::TryFillIsland()
{
	return false;
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
}

void AGuardianTile::BeginPlay()
{
	Super::BeginPlay();
}