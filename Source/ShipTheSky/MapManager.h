// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapManager.generated.h"

/**
 * 
 */

struct Cell
{
	class ABaseTile* Tile;
	class ABaseTile* Parent;
	int32 f;
	int32 g;
	int32 h;
};

UCLASS()
class SHIPTHESKY_API UMapManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMapManager();

	//�� ����
	UFUNCTION(BlueprintCallable)
	void GenerateMap(int32 NumCol);

	//��� Ÿ�Ͽ� ��¥ ȿ�� ����
	UFUNCTION()
	void TimePassesToAllTile(int32 GameDate);

	//���� �� Ÿ�� ��ȯ
	UFUNCTION()
	void GetSameIslandTiles(int32 IslandID, TArray<class AIslandTile*>& OutArray) const;

	//��ȣ�� Ÿ��(���� Ÿ��) ��ȯ
	UFUNCTION()
	class AGuardianTile* GetGuardianTile(int32 IslandID) const;

	//���� Ÿ�� ��ȯ
	UFUNCTION()
	void GetAdjacentTiles(class ABaseTile* Tile, TArray<class ABaseTile*>& OutArray) const;

	//��� ����
	bool GetPathForTile(class AShip* Ship, class ABaseTile* EndTile, bool bIsForBattle);

	//�Ÿ� ���
	int32 GetDistanceOfTwoTile(class ABaseTile* Tile1, class ABaseTile* Tile2);

private:
	void SetTilePowers();
	void SetIslandResources();
	void SetStartLocation();
	void SelectAllIslandTiles();

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

	//��� Ÿ��
	TArray<TArray<class ABaseTile*>> Map;
	//�� Ÿ��
	TArray<TArray<class AIslandTile*>> IslandTiles;

	int32 RowOffset[18] = { -1, -1, 0, 0, 1, 1, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2 };
	int32 ColOffset[2][18] = { {0, 1, -1, 1, 0, 1, -1, 0, 1, -1, 2, -2, 2, -1, 2, -1, 0, 1},
					{-1, 0, -1, 1, -1, 0, -1, 0, 1, -2, 1, -2, 2, -2, 1, -1, 0, 1} };
};
