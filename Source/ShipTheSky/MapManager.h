// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapManager.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UMapManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMapManager();

	UFUNCTION(BlueprintCallable)
	void GenerateMap(int32 NumCol);

	UFUNCTION(BlueprintCallable)
	void ClearMap();

	UFUNCTION()
	void GiveTileEffectsToAll();

	TArray<class AIslandTile*> GetSameIslandTiles(int32 IslandID) const;

private:
	void TempSetStartLocation();

private:
	TSubclassOf<class AIslandTile> MineTileClass;
	TSubclassOf<class AIslandTile> ForestTileClass;
	TSubclassOf<class AIslandTile> FarmTileClass;
	TSubclassOf<class AIslandTile> GuardianTileClass;
	TSubclassOf<class ABaseTile> CloudTileClass;
	TSubclassOf<class ABaseTile> StormTileClass;
	TSubclassOf<class ABaseTile> SunTileClass;
	TSubclassOf<class ABaseTile> LightningTileClass;
	TSubclassOf<class ABaseTile> MeteorTileClass;

	TArray<TArray<class ABaseTile*>> Map;
	TArray<TArray<class AIslandTile*>> IslandTiles;

	int32 RowOffset[18] = { -1, -1, 0, 0, 1, 1, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2 };
	int32 ColOffset[2][18] = { {0, 1, -1, 1, 0, 1, -1, 0, 1, -1, 2, -2, 2, -1, 2, -1, 0, 1},
					{-1, 0, -1, 1, -1, 0, -1, 0, 1, -2, 1, -2, 2, -2, 1, -1, 0, 1} };
};
