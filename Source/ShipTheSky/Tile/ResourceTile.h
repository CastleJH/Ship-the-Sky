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
	AResourceTile();

	//아마 고칠지도?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<uint8, int32> Resources;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Widget", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* ResourcesWidgetComp;

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

	virtual void OnTileReleased(AActor* Target, FKey ButtonPressed) override;

	void UpdateTileResourcesUI();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateTileResourcesUI();

protected:
	virtual void BeginPlay() override;
	void GiveResourceToUnit();
	void GiveProgressToBuilding();
};
