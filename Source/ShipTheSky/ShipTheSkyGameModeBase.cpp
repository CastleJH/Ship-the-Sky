// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShipTheSkyGameModeBase.h"
#include "MapManager.h"
#include "STSGameState.h"

void AShipTheSkyGameModeBase::StartPlay()
{
	Super::StartPlay();
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
		GetWorld()->GetTimerManager().SetTimer(STSGameState->DateTimer, STSGameState, &ASTSGameState::SetGameDate, 1.0f, true, 1.0f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("World Not Prepared"));
	}
}