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

	//맵 생성
	void GenerateMap();

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

	//경로 생성
	bool MakePathToTile(class AShip* Ship, class ABaseTile* EndTile, bool bIsForBattle);

	//거리 계산
	static int32 GetDistanceOfTwoTile(class ABaseTile* Tile1, class ABaseTile* Tile2);

	//배가 없으면서, 적 섬타일이 아니면서, 적대적인 수호자의 인접타일이 아닌지 반환
	bool GetIsTileAccessible(class ACommander* Commander, class ABaseTile* Tile);

	//맵에서 자원 UI 끄고 키기
	bool SetResoureUIVisibility(bool bIsVisible);

	FVector GetMaxCameraLocation() const;
	float GetMaxCameraSpringArm() const;

private:
	void SetTilePowers();
	void SetIslandResources();
	void SetStartLocation();
	void SelectAllIslandTiles();

private:
	//모든 타일
	TArray<TArray<class ABaseTile*>> Map;
	//섬 타일
	TArray<TArray<class AIslandTile*>> IslandTiles;

	int32 RowOffset[18] = { -1, -1, 0, 0, 1, 1, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2 };
	int32 ColOffset[2][18] = { {0, 1, -1, 1, 0, 1, -1, 0, 1, -1, 2, -2, 2, -1, 2, -1, 0, 1},
					{-1, 0, -1, 1, -1, 0, -1, 0, 1, -2, 1, -2, 2, -2, 1, -1, 0, 1} };

public:
	//거리순 정렬
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
