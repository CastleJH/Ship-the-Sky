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
};

struct PQElem
{
	int32 f;
	class ABaseTile* Tile;

	bool operator <(const PQElem& A) const
	{
		return f < A.f;
	}
};

UCLASS()
class SHIPTHESKY_API UMapManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMapManager();

	//�� ����
	void GenerateMap();

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
	bool MakePathToTile(class AShip* Ship, class ABaseTile* EndTile, bool bIsForBattle);

	//�Ÿ� ���
	static int32 GetDistanceOfTwoTile(class ABaseTile* Tile1, class ABaseTile* Tile2);

	//�谡 �����鼭, �� ��Ÿ���� �ƴϸ鼭, �������� ��ȣ���� ����Ÿ���� �ƴ��� ��ȯ
	bool GetIsTileAccessible(class ACommander* Commander, class ABaseTile* Tile);

	//�ʿ��� �ڿ� UI ���� Ű��
	bool SetResoureUIVisibility(bool bIsVisible);

	FVector GetMaxCameraLocation() const;
	float GetMaxCameraSpringArm() const;

private:
	void SetTilePowers();
	void SetIslandResources();
	void SetStartLocation();
	void SelectAllIslandTiles();

private:
	//��� Ÿ��
	TArray<TArray<class ABaseTile*>> Map;
	//�� Ÿ��
	TArray<TArray<class AIslandTile*>> IslandTiles;

	int32 RowOffset[18] = { -1, -1, 0, 0, 1, 1, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2 };
	int32 ColOffset[2][18] = { {0, 1, -1, 1, 0, 1, -1, 0, 1, -1, 2, -2, 2, -1, 2, -1, 0, 1},
					{-1, 0, -1, 1, -1, 0, -1, 0, 1, -2, 1, -2, 2, -2, 1, -1, 0, 1} };

public:
	//�Ÿ��� ����
	TArray<TArray<TPair<int32, class AGuardianTile*>>> NearestIslands;
	UPROPERTY()
	TArray<class UWidgetComponent*> ResourceWidgetComps;

	UPROPERTY()
	TSubclassOf<class AIslandTile> MineTileClass;
	UPROPERTY()
	TSubclassOf<class AIslandTile> ForestTileClass;
	UPROPERTY()
	TSubclassOf<class AIslandTile> FarmTileClass;
	UPROPERTY()
	TSubclassOf<class AIslandTile> GuardianTileClass;
	UPROPERTY()
	TSubclassOf<class ABaseTile> CloudTileClass;
	UPROPERTY()
	TSubclassOf<class ABaseTile> StormTileClass;
	UPROPERTY()
	TSubclassOf<class ABaseTile> SunTileClass;
	UPROPERTY()
	TSubclassOf<class ABaseTile> LightningTileClass;
	UPROPERTY()
	TSubclassOf<class ABaseTile> MeteorTileClass;

	UPROPERTY(BlueprintReadWrite)
	int32 NumColOfTheMap;
	UPROPERTY(BlueprintReadWrite)
	float WorldEnemyWeight;
	UPROPERTY(BlueprintReadWrite)
	int32 EndYear;
};
