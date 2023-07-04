// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnitAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UUnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UUnitAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsIdle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsWalking;	
};
