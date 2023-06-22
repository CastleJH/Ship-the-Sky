// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShipTheSkyGameModeBase.h"
#include "MapManager.h"
#include "STSGameState.h"

void AShipTheSkyGameModeBase::StartPlay()
{
	Super::StartPlay();
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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn"));
	}

	GetGameInstance()->GetSubsystem<UMapManager>()->GenerateMap(60);
	ASTSGameState* STSGameState = Cast<ASTSGameState>(GetWorld()->GetGameState());

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