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
	//Ÿ�� ���� ����(Ÿ�� ���� �ִ� ��)
	UPROPERTY(VisibleAnywhere)
	class ABaseBuilding* BuildingOnThisTile = nullptr;
	UPROPERTY(VisibleAnywhere)
	class ABaseUnit* UnitOnThisTile = nullptr;

public:
	//Ÿ�� ���� ����(�ڿ�)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<uint8, float> Resources;

public:
	//Ÿ�� ���� ���� ����
	void SetBuilding(class ABaseBuilding* NewBuilding) { BuildingOnThisTile = NewBuilding; }
	UFUNCTION(BlueprintPure)
	class ABaseBuilding* GetBuilding() const { return BuildingOnThisTile; }
	void SetUnit(class ABaseUnit* NewUnit) { UnitOnThisTile = NewUnit; }
	UFUNCTION(BlueprintPure)
	class ABaseUnit* GetUnit() const { return UnitOnThisTile; }
	void SetResources(float Power);

	//Ÿ�� ȿ������
	virtual void TimePass(int32 GameDate) override;

	//Ÿ�� �ڿ� UI ����
	void UpdateTileResourcesUI();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateTileResourcesUI();

protected:
	virtual void BeginPlay() override;

	//Ÿ�Ͽ��� �� ��¥���� �Ͼ�� �ϵ�
	void GiveResourceToUnit();
	void GiveProgressToBuilding();
};
