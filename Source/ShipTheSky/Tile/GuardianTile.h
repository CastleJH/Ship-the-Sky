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
	
protected:
	virtual void OnTileSelectedAsView(class ASTSPlayerController* PlayerController) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class ABaseUnit*> UnitsOnThisIsland;
	TArray<TSubclassOf<AActor>> Guardians;

	//수호자
	class AGuardian* Guardian;

private:
	UPROPERTY()
	TArray<class AShip*> EnemyShips;
	UPROPERTY()
	TArray<class ABaseTile*> AdjTiles;
	UPROPERTY()
	TArray<class AResourceTile*> AdjResourceTiles;
	int32 RecoveryCount;

public:
	void AddUnitOnThisIsland(class ABaseUnit* Unit);
	void RemoveUnitFromThisIsland(class ABaseUnit* Unit);
	UFUNCTION(BlueprintPure)
	class AGuardian* GetGuardian() const { return Guardian; }

	virtual void TimePass(int32 GameDate) override;

	//수호자 & 전투 관련
	void SpawnGuardian(int32 Index);
	bool CheckEnemyShipAdjacent();
	void AttackShips();
	void GetAttackedByShips();
	UFUNCTION()
	class ACommander* GetNewOwner();
	void RecoverFriendly();
	bool TryFillIsland();

	//인접 타일 정보
	void SetAdjTiles(TArray<class ABaseTile*>& InTile);
	void SetAdjResourceTiles(TArray<class AResourceTile*>& InTile);

protected:
	virtual void BeginPlay() override;
};
