// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/IslandTile.h"
#include "GuardianTile.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API AGuardianTile : public AIslandTile
{
	GENERATED_BODY()
	
public:
	AGuardianTile();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class ABaseUnit*> UnitsOnThisIsland;

	//��ȣ��
	class AGuardian* Guardian;

private:
	UPROPERTY()
	TArray<class AShip*> EnemyShips;
	int32 RecoveryCount;
	UPROPERTY()
	TArray<class ABaseTile*> AdjTiles;
	UPROPERTY()
	TArray<class AResourceTile*> AdjResourceTiles;

public:
	void AddUnitOnThisIsland(class ABaseUnit* Unit);
	void RemoveUnitFromThisIsland(class ABaseUnit* Unit);
	UFUNCTION(BlueprintPure)
	class AGuardian* GetGuardian() const { return Guardian; }

	virtual void TimePass(int32 GameDate) override;

	//��ȣ�� & ���� ����
	void SpawnGuardian(int32 Index);
	bool CheckEnemyShipAdjacent();
	void AttackShips();
	void GetAttackedByShips();
	UFUNCTION()
	class ACommander* GetNewOwner();
	void RecoverFriendly();
	bool TryFillIsland();

	//���� Ÿ�� ����
	void SetAdjTiles(TArray<class ABaseTile*>& InTile);
	void SetAdjResourceTiles(TArray<class AResourceTile*>& InTile);
	TArray<class AResourceTile*>& GetAdjResourceTiles() { return AdjResourceTiles; }

protected:
	virtual void BeginPlay() override;
};
