// Fill out your copyright notice in the Description page of Project Settings.


#include "STSGameState.h"
#include "MapManager.h"

void ASTSGameState::ResetIslandOwner(int32 NewIslandNum, bool bPlayerOwnAllIsland)
{
	if (bPlayerOwnAllIsland) IslandOwner.Init(GetWorld()->GetFirstPlayerController(), NewIslandNum);
	else IslandOwner.Init(nullptr, NewIslandNum);
}

AController* ASTSGameState::GetIslandOwner(int32 IslandID) const
{
	if (IslandOwner.IsValidIndex(IslandID))
	{
		return IslandOwner[IslandID];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Island Idx"));
		return nullptr;
	}
}

void ASTSGameState::SetIslandOwner(int32 IslandID, AController* NewOwner)
{
	if (IslandOwner.IsValidIndex(IslandID))
	{
		IslandOwner[IslandID] = NewOwner;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Island Idx"));
	}
}

void ASTSGameState::ResetGameDate()
{
	GameDateInt32 = 2199 * 360;

	int32 Year = GameDateInt32 / 360 + 1;
	int32 Month = (GameDateInt32 / 30) % 12 + 1;
	int32 Day = GameDateInt32 % 30 + 1;

	GameDateString = FString::Printf(TEXT("%d.%02d.%02d"), Year, Month, Day);
}

void ASTSGameState::IncreaseGameDate()
{
	GameDateInt32++;

	int32 Year = GameDateInt32 / 360 + 1;
	int32 Month = (GameDateInt32 / 30) % 12 + 1;
	int32 Day = GameDateInt32 % 30 + 1;

	GameDateString = FString::Printf(TEXT("%d.%02d.%02d"), Year, Month, Day);

	GetGameInstance()->GetSubsystem<UMapManager>()->TimePassesToAllTile(GameDateInt32);
}
