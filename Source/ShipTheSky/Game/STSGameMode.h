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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIslandTile> MineTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIslandTile> ForestTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIslandTile> FarmTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIslandTile> GuardianTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseTile> CloudTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseTile> StormTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseTile> SunTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseTile> LightningTileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseTile> MeteorTileClass;
	
};
