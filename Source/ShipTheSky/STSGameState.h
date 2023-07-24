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

public:
	ASTSGameState();

private:
	UPROPERTY(VisibleAnywhere)
	TArray<class ACommander*> IslandOwner;
	int32 GameDateInt32;
	FString GameDateString;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class ACommander*> Commanders;
	TArray<int32> CommanderScores;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGameOver;
	UPROPERTY(BlueprintReadOnly)
	bool bIsPlayerTheWinner;
	int32 EndYear;

public:
	//�� ������ �ʱ�ȭ
	void ResetIslandOwner(int32 NewIslandNum, int32 PlayerOwnAmount, bool bPlayerToo);

	//�� ������ ��ȯ
	UFUNCTION(BlueprintPure)
	ACommander* GetIslandOwner(int32 IslandID) const;
	UFUNCTION(BlueprintPure)
	int32 GetCommanderScore(int32 Idx) const { return CommanderScores[Idx]; }
	
	//�� ������ ����
	void SetIslandOwner(int32 IslandID, class ACommander* NewOwner);

	//���� ��¥ �ʱ�ȭ
	void ResetGameDate();

	//���� ��¥ ��ȯ
	UFUNCTION(BlueprintPure)
	FString GetGameDate() const { return GameDateString; }

	//���� ��¥ ����
	void IncreaseGameDate();

	FTimerHandle DateTimer;
};
