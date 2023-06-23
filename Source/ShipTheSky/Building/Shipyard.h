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
	//������� ���༱��
	TArray<FString> WaitingShipArray;

	//���༱ ���� ���� ����
	int32 TimeNeed;
	bool bIsCreatingShip;
	int32 MaxWaitingShip = 10;

public:
	//���༱ ����
	UFUNCTION()
	bool AddShipCreationToArray();
	//���༱ ���
	UFUNCTION(BlueprintCallable)
	void CancelWaitingShipByIndex(int32 Index);
	//���� ��Ȳ ������Ʈ
	virtual void IncreaseProgress() override;

	//UI ���� ���� ����
	UFUNCTION(BlueprintPure)
	FString GetWaitingShipNameByIndex(int32 Index) const;
	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;
	UFUNCTION(BlueprintPure)
	int32 GetLeftDate() const { return FMath::Max(0, TimeNeed - Progress); }

private:
	//���༱ ���� ����
	void StartShipCreation();
	bool FinishShipCreation();
	bool CreateShip(FString ShipName);
};
