// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BaseBuilding.h"
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
	const int32 MaxWaitingUnit = 10;

private:
	//대기중인 유닛들
	TArray<enum EUnitType> WaitingUnitArray;

	//유닛 생성 관련 변수
	int32 TimeNeed;
	bool bIsCreatingUnit;

public:
	//유닛 생성
	UFUNCTION()
	bool AddUnitCreationToArray(enum EUnitType UnitType);
	//유닛 취소
	UFUNCTION(BlueprintCallable)
	void CancelWaitingUnitByIndex(int32 Index);
	//진행 상황 업데이트
	virtual void IncreaseProgress() override;

	//UI 위한 변수 접근
	UFUNCTION(BlueprintPure)
	enum EUnitType GetWaitingUnitByIndex(int32 Index) const;
	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;
	UFUNCTION(BlueprintPure)
	int32 GetLeftDate() const { return TimeNeed - Progress; }
	UFUNCTION(BlueprintPure)
	FString GetCreatingUnitName() const;

private:
	//유닛 생성 관련
	void StartUnitCreation();
	void FinishUnitCreation();
	class ABaseUnit* CreateUnit(EUnitType Type);
};
