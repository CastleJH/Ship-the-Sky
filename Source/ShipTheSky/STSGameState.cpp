// Fill out your copyright notice in the Description page of Project Settings.


#include "STSGameState.h"

void ASTSGameState::ResetIslandOwner(int32 NewIslandNum, bool bPlayerOwnAllIsland)
{
	if (bPlayerOwnAllIsland) IslandOwner.Init(GetWorld()->GetFirstPlayerController(), NewIslandNum);
	else IslandOwner.Init(nullptr, NewIslandNum);
	for (auto elem : Actors) elem->Destroy();
	Actors.Empty();
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
