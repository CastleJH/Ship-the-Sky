// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/IslandTile.h"
#include "ResourceTile.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API AResourceTile : public AIslandTile
{
	GENERATED_BODY()

public:
	//고쳐요 나중에
	UPROPERTY(VisibleAnywhere)
	TMap<uint8, int32> Resources;

private:
	UPROPERTY()
	class ABaseBuilding* BuildingOnThisTile = nullptr;
	UPROPERTY()
	class ABaseUnit* UnitOnThisTile = nullptr;

public:
	void SetBuilding(class ABaseBuilding* NewBuilding) { BuildingOnThisTile = NewBuilding; }
	UFUNCTION(BlueprintPure)
	class ABaseBuilding* GetBuilding() const { return BuildingOnThisTile; }

	void SetUnit(class ABaseUnit* NewUnit) { UnitOnThisTile = NewUnit; }
	UFUNCTION(BlueprintPure)
	class ABaseUnit* GetUnit() const { return UnitOnThisTile; }

	void SetResources(float Power);

	virtual void TimePass(int32 GameDate) override;

protected:
	virtual void OnTileReleased(AActor* Target, FKey ButtonPressed) override;

	void GiveResourceToUnit();
	void GiveProgressToBuilding();
};
