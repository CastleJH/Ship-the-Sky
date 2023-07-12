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
	TArray<class ACommander*> IslandOwner;
	int32 GameDateInt32;
	FString GameDateString;

public:
	UPROPERTY(VisibleAnywhere)
	TArray<class ACommander*> Commanders;

public:
	//섬 소유권 초기화
	void ResetIslandOwner(int32 NewIslandNum, int32 PlayerOwnAmount);

	//섬 소유자 반환
	UFUNCTION(BlueprintPure)
	ACommander* GetIslandOwner(int32 IslandID) const;
	
	//섬 소유자 설정
	void SetIslandOwner(int32 IslandID, class ACommander* NewOwner);

	//게임 날짜 초기화
	void ResetGameDate();

	//게임 날짜 반환
	UFUNCTION(BlueprintPure)
	FString GetGameDate() const { return GameDateString; }

	//게임 날짜 증가
	void IncreaseGameDate();

	FTimerHandle DateTimer;
};
