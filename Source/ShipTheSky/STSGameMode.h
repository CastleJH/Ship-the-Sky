// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API ASTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class ACommander>> EnemyPlayerClasses;
	
};
