// Fill out your copyright notice in the Description page of Project Settings.


#include "STSGameState.h"
#include "MapManager.h"
#include "Pawn/Commander.h"
#include "GameFramework/WorldSettings.h"

ASTSGameState::ASTSGameState()
{
	bIsGameOver = false;
	bIsPlayerTheWinner = false;
	EndYear = 2250;
}

void ASTSGameState::ResetIslandOwner(int32 NewIslandNum, int32 PlayerOwnAmount, bool bPlayerToo)
{
	IslandOwner.Init(nullptr, NewIslandNum);
	int32 Idx = Commanders.Num();
	if (bPlayerToo)
	{
		for (auto Commander : Commanders)
		{
			for (int32 Cnt = 0; Cnt < PlayerOwnAmount; Cnt++)
			{
				SetIslandOwner(Idx, Commander);
				Idx++;
			}
		}
	}
	else
	{
		for (auto Commander : Commanders)
		{
			if (Commander->CommanderID == 0) continue;
			for (int32 Cnt = 0; Cnt < PlayerOwnAmount; Cnt++)
			{
				SetIslandOwner(Idx, Commander);
				Idx++;
			}
		}
	}
}

ACommander* ASTSGameState::GetIslandOwner(int32 IslandID) const
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

void ASTSGameState::SetIslandOwner(int32 IslandID, ACommander* NewOwner)
{
	if (IslandOwner.IsValidIndex(IslandID))
	{
		AGuardianTile* Tile = GetGameInstance()->GetSubsystem<UMapManager>()->GetGuardianTile(IslandID);
		if (IslandOwner[IslandID])
		{
			IslandOwner[IslandID]->OwningIslands.Remove(Tile);
		}
		IslandOwner[IslandID] = NewOwner;
		NewOwner->OwningIslands.Add(Tile);
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

	UE_LOG(LogTemp, Warning, TEXT("New Day"));

	if (GameDateInt32 >= (EndYear - 1) * 360)
	{
		if (CommanderScores.Num() != 5)
		{
			UE_LOG(LogTemp, Error, TEXT("Score length not 5."));
			return;
		}
		if (Commanders[0]->OwningIslands.Num() == 0)
		{
			bIsPlayerTheWinner = false;
			bIsGameOver = true;
		}

		bIsPlayerTheWinner = true;
		for (int32 Idx = 1; Idx < 5; Idx++)
		{
			if (CommanderScores[Idx] > CommanderScores[0])
			{
				bIsPlayerTheWinner = false;
			}
		}
		bIsGameOver = true;
	}
}
