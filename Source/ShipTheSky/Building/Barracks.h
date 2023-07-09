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
	//������� ���ֵ�
	TArray<enum EUnitType> WaitingUnitArray;

	//���� ���� ���� ����
	int32 TimeNeed;
	bool bIsCreatingUnit;

public:
	//���� ����
	UFUNCTION()
	bool AddUnitCreationToArray(enum EUnitType UnitType);
	//���� ���
	UFUNCTION(BlueprintCallable)
	void CancelWaitingUnitByIndex(int32 Index);
	//���� ��Ȳ ������Ʈ
	virtual void IncreaseProgress() override;

	//UI ���� ���� ����
	UFUNCTION(BlueprintPure)
	enum EUnitType GetWaitingUnitByIndex(int32 Index) const;
	UFUNCTION(BlueprintPure)
	float GetProgressRate() const;
	UFUNCTION(BlueprintPure)
	int32 GetLeftDate() const { return TimeNeed - Progress; }
	UFUNCTION(BlueprintPure)
	FString GetCreatingUnitName() const;

private:
	//���� ���� ����
	void StartUnitCreation();
	void FinishUnitCreation();
	class ABaseUnit* CreateUnit(EUnitType Type);
};
