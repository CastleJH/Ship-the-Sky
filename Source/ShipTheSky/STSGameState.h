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
	int32 GameDateInt32;
	FString GameDateString;
	
public:
	//섬 소유권 초기화
	void ResetIslandOwner(int32 NewIslandNum, bool bPlayerOwnAllIsland);

	//섬 소유자 반환
	UFUNCTION(BlueprintPure)
	AController* GetIslandOwner(int32 IslandID) const;
	
	//섬 소유자 설정
	void SetIslandOwner(int32 IslandID, class AController* NewOwner);

	//게임 날짜 초기화
	void ResetGameDate();

	//게임 날짜 반환
	UFUNCTION(BlueprintPure)
	FString GetGameDate() const { return GameDateString; }

	//게임 날짜 증가
	void IncreaseGameDate();

	FTimerHandle DateTimer;
};
