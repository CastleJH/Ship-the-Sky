// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BaseBuilding.h"
#include "Shipyard.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API AShipyard : public ABaseBuilding
{
	GENERATED_BODY()

public:
	AShipyard();

private:
	TArray<TArray<int32>> WaitingShipArray;
	int32 TimeNeed;
	bool bIsCreatingShip;
	int32 MaxWaitingShip = 10;

public:
	UFUNCTION()
	bool AddShipCreationToArray(int32 WoodCloud, int32 WoodStorm, int32 WoodSun, int32 WoodLightning, int32 WoodMeteor);

	UFUNCTION(BlueprintPure)
	int32 GetWaitingShipResourceByIndex(int32 Index, enum EResourceType ResourceType) const;

	UFUNCTION(BlueprintCallable)
	void CancelWaitingShipByIndex(int32 Index);

	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;

	UFUNCTION(BlueprintPure)
	int32 GetLeftDate() const { return TimeNeed - Progress; }

	virtual void IncreaseProgress() override;

private:
	void StartShipCreation();
	void FinishShipCreation();
	void CreateUnit(TArray<int32>& Resources);
};
