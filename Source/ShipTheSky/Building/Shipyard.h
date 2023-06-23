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
	//대기중인 비행선들
	TArray<FString> WaitingShipArray;

	//비행선 생성 관련 변수
	int32 TimeNeed;
	bool bIsCreatingShip;
	int32 MaxWaitingShip = 10;

public:
	//비행선 생성
	UFUNCTION()
	bool AddShipCreationToArray();
	//비행선 취소
	UFUNCTION(BlueprintCallable)
	void CancelWaitingShipByIndex(int32 Index);
	//진행 상황 업데이트
	virtual void IncreaseProgress() override;

	//UI 위한 변수 접근
	UFUNCTION(BlueprintPure)
	FString GetWaitingShipNameByIndex(int32 Index) const;
	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;
	UFUNCTION(BlueprintPure)
	int32 GetLeftDate() const { return FMath::Max(0, TimeNeed - Progress); }

private:
	//비행선 생성 관련
	void StartShipCreation();
	bool FinishShipCreation();
	bool CreateShip(FString ShipName);
};
