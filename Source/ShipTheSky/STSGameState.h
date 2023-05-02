// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STSGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API ASTSGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TArray<class AController*> IslandOwner;

public:
	void ResetIslandOwner(int32 NewIslandNum, bool bPlayerOwnAllIsland);
	AController* GetIslandOwner(int32 IslandID) const;
	void SetIslandOwner(int32 IslandID, class AController* NewOwner);

	TArray<AActor*> Actors;
};
