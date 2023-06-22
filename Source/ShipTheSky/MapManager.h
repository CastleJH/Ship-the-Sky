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

	//맵 생성
	UFUNCTION(BlueprintCallable)
	void GenerateMap(int32 NumCol);

	//모든 타일에 날짜 효과 적용
	UFUNCTION()
	void TimePassesToAllTile(int32 GameDate);

	//같은 섬 타일 반환
	UFUNCTION()
	void GetSameIslandTiles(int32 IslandID, TArray<class AIslandTile*>& OutArray) const;

	//수호자 타일(메인 타일) 반환
	UFUNCTION()
	class AGuardianTile* GetGuardianTile(int32 IslandID) const;

	//인접 타일 반환
	UFUNCTION()
	void GetAdjacentTiles(class ABaseTile* Tile, TArray<class ABaseTile*>& OutArray) const;

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

	//모든 타일
	TArray<TArray<class ABaseTile*>> Map;
	//섬 타일
	TArray<TArray<class AIslandTile*>> IslandTiles;

	int32 RowOffset[18] = { -1, -1, 0, 0, 1, 1, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2 };
	int32 ColOffset[2][18] = { {0, 1, -1, 1, 0, 1, -1, 0, 1, -1, 2, -2, 2, -1, 2, -1, 0, 1},
					{-1, 0, -1, 1, -1, 0, -1, 0, 1, -2, 1, -2, 2, -2, 1, -1, 0, 1} };
};
