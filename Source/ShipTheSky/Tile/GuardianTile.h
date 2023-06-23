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

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> Guardians;
public:
	void AddUnitOnThisIsland(class ABaseUnit* Unit);
	void RemoveUnitFromThisIsland(class ABaseUnit* Unit);

protected:
	virtual void BeginPlay() override;
};
