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

protected:
	UPROPERTY(VisibleAnywhere, Category = "Widget", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> ResourcesWidgetComp;

private:
	//타일 관련 변수(타일 위에 있는 것)
	UPROPERTY(VisibleAnywhere)
	class ABaseBuilding* BuildingOnThisTile = nullptr;
	UPROPERTY(VisibleAnywhere)
	class ABaseUnit* UnitOnThisTile = nullptr;

public:
	//타일 관련 변수(자원)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<uint8, float> Resources;

public:
	//타일 관련 변수 접근
	void SetBuilding(class ABaseBuilding* NewBuilding) { BuildingOnThisTile = NewBuilding; }
	UFUNCTION(BlueprintPure)
	class ABaseBuilding* GetBuilding() const { return BuildingOnThisTile; }
	void SetUnit(class ABaseUnit* NewUnit) { UnitOnThisTile = NewUnit; }
	UFUNCTION(BlueprintPure)
	class ABaseUnit* GetUnit() const { return UnitOnThisTile; }
	void SetResources(float Power);

	//타일 효과적용
	virtual void TimePass(int32 GameDate) override;

	//타일 자원 UI 설정
	void UpdateTileResourcesUI();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateTileResourcesUI();

protected:
	virtual void BeginPlay() override;

	//타일에서 매 날짜마다 일어나는 일들
	void GiveResourceToUnit();
	void GiveProgressToBuilding();
};
