// Fill out your copyright notice in the Description page of Project Settings.


#include "STSGameMode.h"
#include "MapManager.h"
#include "STSGameState.h"
#include "Pawn/Commander.h"

void ASTSGameMode::StartPlay()
{
	Super::StartPlay();

	ASTSGameState* STSGameState = Cast<ASTSGameState>(GetWorld()->GetGameState());
	if (GetWorld()->GetFirstPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetWorld()->GetFirstPlayerController()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Controller"));
	}

	if (GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetWorld()->GetFirstPlayerController()->GetPawn()->GetName());
		STSGameState->Commanders.Add(Cast<ACommander>(GetWorld()->GetFirstPlayerController()->GetPawn()));
		check(STSGameState->Commanders.Last() != nullptr)
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn"));
	}

	for (auto Class : EnemyPlayerClasses)
	{
		STSGameState->Commanders.Add(GetWorld()->SpawnActor<ACommander>(Class));
		check(STSGameState->Commanders.Last() != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("%s"), *STSGameState->Commanders.Last()->GetName());
	}
	STSGameState->CommanderScores.Init(0, STSGameState->Commanders.Num());

	GetGameInstance()->GetSubsystem<UMapManager>()->GenerateMap(60);

	if (GameState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No GameState"));
		return;
	}

	//세계 시간 타이머
	if (GetWorld())
	{
		STSGameState->ResetGameDate();
		GetWorld()->GetTimerManager().SetTimer(STSGameState->DateTimer, STSGameState, &ASTSGameState::IncreaseGameDate, 1.0f, true, 1.0f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World Not Prepared"));
	}
}
