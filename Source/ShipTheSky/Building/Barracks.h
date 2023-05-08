// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BaseBuilding.h"
#include "Containers/Queue.h"
#include "Barracks.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API ABarracks : public ABaseBuilding
{
	GENERATED_BODY()
	
public:
	ABarracks();

private:
	TQueue<enum EUnitType> WaitingUnitCreation;
	int32 TimeNeed;
	bool bIsCreatingUnit;

public:
	UFUNCTION()
	void AddUnitCreationToQueue(enum EUnitType UnitType);

	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;

	UFUNCTION(BlueprintPure)
	int32 GetLeftDate() const { return TimeNeed - Progress; }

	UFUNCTION(BlueprintPure)
	FString GetCreatingUnitName() const;

	virtual void IncreaseProgress() override;

private:
	void StartUnitCreation();
	void FinishUnitCreation();
};
