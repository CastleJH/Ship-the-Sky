// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "STSGameState.h"
#include "Tile/IslandTile.h"
#include "Tile/ResourceTile.h"
#include "Tile/GuardianTile.h"
#include "Pawn/PlayerCommander.h"
#include "Controller/STSPlayerController.h"
#include "Enums.h"

UMapManager::UMapManager()
{
	MineTileClass = LoadClass<AIslandTile>(nullptr, TEXT("/Game/Tile/Island/BP_MineTile.BP_MineTile_C"));
	ForestTileClass = LoadClass<AIslandTile>(nullptr, TEXT("/Game/Tile/Island/BP_ForestTile.BP_ForestTile_C"));
	FarmTileClass = LoadClass<AIslandTile>(nullptr, TEXT("/Game/Tile/Island/BP_FarmTile.BP_FarmTile_C"));
	GuardianTileClass = LoadClass<AIslandTile>(nullptr, TEXT("/Game/Tile/Island/BP_GuardianTile.BP_GuardianTile_C"));
	CloudTileClass = LoadClass<ABaseTile>(nullptr, TEXT("/Game/Tile/Sky/BP_CloudTile.BP_CloudTile_C"));
	StormTileClass = LoadClass<ABaseTile>(nullptr, TEXT("/Game/Tile/Sky/BP_StormTile.BP_StormTile_C"));
	SunTileClass = LoadClass<ABaseTile>(nullptr, TEXT("/Game/Tile/Sky/BP_SunTile.BP_SunTile_C"));
	LightningTileClass = LoadClass<ABaseTile>(nullptr, TEXT("/Game/Tile/Sky/BP_LightningTile.BP_LightningTile_C"));
	MeteorTileClass = LoadClass<ABaseTile>(nullptr, TEXT("/Game/Tile/Sky/BP_MeteorTile.BP_MeteorTile_C"));
}

void UMapManager::GenerateMap(int32 NumCol)
{
	int32 NumRow = (int32)((float)NumCol * 0.75f);
	int32 CurR, CurC, NewR, NewC, MaxPoss;
	int32 NewIslandID = 0;
	int32 Iter = 8;
	ETileType NewType, NearType;

	TArray<TArray<ETileType>> MapCalc;
	TArray<TArray<ETileType>> MapData;
	TArray<TArray<int32>> IslandID;

	//랜덤하게 타일들의 시작 속성 채우기
	for (int32 Row = 0; Row < NumRow; Row++)
	{
		TArray<ETileType> NewRow;
		TArray<int32> IslandIDRow;
		for (int32 Col = 0; Col < NumCol; Col++)
		{
			NewRow.Add(ETileType(FMath::RandRange(1, (int32)ETileType::None - 1)));
			IslandIDRow.Add(-1);
		}
		MapData.Add(NewRow);
		IslandID.Add(IslandIDRow);
	}

	//모든 타일에 대해 인접한 타일과 비교하여 현재 타일의 속성 갱신
	while (Iter--)
	{
		MapCalc = MapData;
		for (CurR = 0; CurR < NumRow; CurR++)
			for (CurC = 0; CurC < NumCol; CurC++)
			{
				if (!MapData.IsValidIndex(CurR) || !MapData[CurR].IsValidIndex(CurC)) continue;

				int32 Poss[(int32)ETileType::None + 1] = { 0 };
				MaxPoss = 0;
				for (int i = 0; i < 6; i++)
				{
					NewR = CurR + RowOffset[i];
					NewC = CurC + ColOffset[CurR % 2][i];

					if (!MapCalc.IsValidIndex(NewR) || !MapCalc[NewR].IsValidIndex(NewC)) continue;

					NearType = MapCalc[NewR][NewC];
					Poss[(int32)NearType]++;
					if (MaxPoss < Poss[(int32)NearType])
					{
						MaxPoss = Poss[(int32)NearType];
						NewType = NearType;
					}
				}
				MapData[CurR][CurC] = NewType;
			}
	}

	//섬 생성
	NewIslandID = 0;
	for (int32 Col = 0; Col < NumCol; Col++)
	{
		int32 IterIsland = 2;
		while (IterIsland--)
		{
			int32 RandRow;
			bool NearCheck;
			int32 maxIter = 100;

			do {
				RandRow = FMath::RandRange(0, NumRow - 1);
				NearCheck = false;
				if (!MapData.IsValidIndex(RandRow) || !MapData[RandRow].IsValidIndex(Col))
				{
					UE_LOG(LogTemp, Warning, TEXT("Wrong Index"));
					return;
				}

				ETileType& CurTile = MapData[RandRow][Col];

				for (int32 i = 0; i < 18; i++)
				{
					NewR = RandRow + RowOffset[i];
					NewC = Col + ColOffset[RandRow % 2][i];

					if (!MapData.IsValidIndex(NewR) || !MapData[NewR].IsValidIndex(NewC)) continue;
					if (MapData[NewR][NewC] == ETileType::Island)
					{
						NearCheck = true;
						break;
					}
				}
			} while (NearCheck && maxIter--);

			if (maxIter == 0) {
				UE_LOG(LogTemp, Error, TEXT("Wrong Map Generation"));
				return;
			}

			ETileType& CurTile = MapData[RandRow][Col];
			CurTile = ETileType::Island;
			IslandID[RandRow][Col] = NewIslandID + 10000;
			int32 IslandTileNum = 0;
			for (int32 i = 0; i < 6; i++)
			{
				NewR = RandRow + RowOffset[i];
				NewC = Col + ColOffset[RandRow % 2][i];

				if (!MapData.IsValidIndex(NewR) || !MapData[NewR].IsValidIndex(NewC)) continue;
				if (FMath::FRandRange(0.f, 1.f) < 0.6f)
				{
					IslandID[NewR][NewC] = NewIslandID;
					MapData[NewR][NewC] = ETileType::Island;
					IslandTileNum++;
				}
				if (IslandTileNum == 5) break;
			}
			while (IslandTileNum == 0)
			{
				int32 RandLoc = FMath::RandRange(0, 5);
				NewR = RandRow + RowOffset[RandLoc];
				NewC = Col + ColOffset[RandRow % 2][RandLoc];

				if (!MapData.IsValidIndex(NewR) || !MapData[NewR].IsValidIndex(NewC)) continue;
				IslandID[NewR][NewC] = NewIslandID;
				MapData[NewR][NewC] = ETileType::Island;
				IslandTileNum++;
			}

			NewIslandID++;
			TArray<AIslandTile*> SameIslands;
			IslandTiles.Add(SameIslands);
		}
	}

	//수호자 타일 먼저 스폰
	float Offs = 500.f;
	float XCoord = 0.0f, YCoord = 0.0f;
	float XDiff = Offs * 0.25f + Offs / FMath::Sqrt(3.0f);
	float YDiff = Offs;
	CurR = 0;
	Map.SetNum(MapData.Num());
	for (auto Row : MapData)
	{
		TArray<ABaseTile*> MapRow;
		MapRow.SetNum(Row.Num());

		YCoord = (CurR & 1) ? -Offs * 0.5f : 0.0f;
		CurC = 0;

		AIslandTile* IslandTile;
		for (auto Elem : Row)
		{
			if (Elem == ETileType::Island)
			{
				if (!IslandID.IsValidIndex(CurR) || !IslandID[CurR].IsValidIndex(CurC))
				{
					UE_LOG(LogTemp, Error, TEXT("Wrong IslandID index."));
					return;
				}
				if (IslandID[CurR][CurC] >= 10000)
				{
					int32 GuardianTileID = IslandID[CurR][CurC] - 10000;
					IslandTile = GetWorld()->SpawnActor<AIslandTile>(GuardianTileClass, FVector(XCoord, YCoord, 60.0f), FRotator::ZeroRotator);
					if (IslandTiles.IsValidIndex(GuardianTileID))
					{
						IslandTiles[GuardianTileID].Insert(IslandTile, 0);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Wrong Island ID %d"), GuardianTileID);
						return;
					}
					IslandTile->SetIslandID(GuardianTileID);
					MapRow[0] = IslandTile;
					IslandTile->SetRow(CurR);
					IslandTile->SetCol(CurC);
				}
			}
			YCoord += YDiff;
			CurC++;
		}
		Map[CurR] = MapRow;
		XCoord += XDiff;
		CurR++;
	}
	XCoord -= XDiff;
	YCoord -= YDiff;

	//나머지 타일 스폰
	Offs = 500.f;
	XCoord = 0.0f, YCoord = 0.0f;
	XDiff = Offs * 0.25f + Offs / FMath::Sqrt(3.0f);
	YDiff = Offs;
	CurR = 0;
	for (auto Row : MapData)
	{
		YCoord = (CurR & 1) ? -Offs * 0.5f : 0.0f;
		CurC = 0;

		ABaseTile* Tile = nullptr;
		AIslandTile* IslandTile = nullptr;
		int32 IslandType;
		for (auto Elem : Row)
		{
			switch (Elem)
			{
			case ETileType::Island:
				if (!IslandID.IsValidIndex(CurR) || !IslandID[CurR].IsValidIndex(CurC))
				{
					UE_LOG(LogTemp, Error, TEXT("Wrong IslandID index."));
					return;
				}
				if (IslandID[CurR][CurC] < 10000)
				{
					if (!IslandTiles.IsValidIndex(IslandID[CurR][CurC]) || !IslandTiles[IslandID[CurR][CurC]].IsValidIndex(0))
					{
						UE_LOG(LogTemp, Error, TEXT("Wrong Guardian tile index."));
						return;
					}
					FRotator Rotator = (IslandTiles[IslandID[CurR][CurC]][0]->GetActorLocation() - FVector(XCoord, YCoord, 60.0f)).Rotation() + FRotator(0.0f, -30.0f, 0.0f);

					IslandType = FMath::RandRange(0, 2);
					if (IslandType == 0) IslandTile = GetWorld()->SpawnActor<AIslandTile>(MineTileClass, FVector(XCoord, YCoord, 60.0f), Rotator);
					else if (IslandType == 1) IslandTile = GetWorld()->SpawnActor<AIslandTile>(ForestTileClass, FVector(XCoord, YCoord, 60.0f), Rotator);
					else IslandTile = GetWorld()->SpawnActor<AIslandTile>(FarmTileClass, FVector(XCoord, YCoord, 60.0f), Rotator);
					IslandTiles[IslandID[CurR][CurC]].Add(IslandTile);
					Cast<AResourceTile>(IslandTile)->SetResources(1.0f);
					IslandTile->SetIslandID(IslandID[CurR][CurC]);
					Tile = IslandTile;
				}
				break;
			case ETileType::Cloud:
				Tile = GetWorld()->SpawnActor<ABaseTile>(CloudTileClass, FVector(XCoord, YCoord, 0.0f), FRotator::ZeroRotator);
				break;
			case ETileType::Storm:
				Tile = GetWorld()->SpawnActor<ABaseTile>(StormTileClass, FVector(XCoord, YCoord, 0.0f), FRotator::ZeroRotator);
				break;
			case ETileType::Sun:
				Tile = GetWorld()->SpawnActor<ABaseTile>(SunTileClass, FVector(XCoord, YCoord, 0.0f), FRotator::ZeroRotator);
				break;
			case ETileType::Lightning:
				Tile = GetWorld()->SpawnActor<ABaseTile>(LightningTileClass, FVector(XCoord, YCoord, 0.0f), FRotator::ZeroRotator);
				break;
			case ETileType::Meteor:
				Tile = GetWorld()->SpawnActor<ABaseTile>(MeteorTileClass, FVector(XCoord, YCoord, 0.0f), FRotator::ZeroRotator);
				break;
			}
			if (Elem != ETileType::Island || IslandID[CurR][CurC] < 10000)
			{
				if (Tile == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("NULL TILE"));
				}
				Map[CurR][CurC] = Tile;
				Tile->SetRow(CurR);
				Tile->SetCol(CurC);
			}
			YCoord += YDiff;
			CurC++;
		}
		XCoord += XDiff;
		CurR++;
	}
	XCoord -= XDiff;
	YCoord -= YDiff;

	GetWorld()->GetGameState<ASTSGameState>()->ResetIslandOwner(NewIslandID, true);
	TempSetStartLocation();
}

void UMapManager::TimePassesToAllTile(int32 GameDate)
{
	for (auto Island : IslandTiles)
	{
		for (auto Tile : Island)
		{
			if (Tile) Tile->TimePass(GameDate);
		}
	}
}

void UMapManager::GetSameIslandTiles(int32 IslandID, TArray<class AIslandTile*>& OutArray) const
{
	if (IslandTiles.IsValidIndex(IslandID))
	{
		OutArray = IslandTiles[IslandID];
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Wrong island id"));
	}
}

AGuardianTile* UMapManager::GetGuardianTile(int32 IslandID) const
{
	if (IslandTiles.IsValidIndex(IslandID) && IslandTiles[IslandID].IsValidIndex(0)) return Cast<AGuardianTile>(IslandTiles[IslandID][0]);
	return nullptr;
}

void UMapManager::GetAdjacentTiles(ABaseTile* Tile, TArray<class ABaseTile*>& OutArray) const
{
	if (Tile)
	{
		int32 Row = Tile->GetRow();
		int32 Col = Tile->GetCol();
		int32 NewRow, NewCol;
		for (int32 i = 0; i < 6; i++)
		{
			NewRow = Row + RowOffset[i];
			NewCol = Col + ColOffset[Row % 2][i];
			if (Map.IsValidIndex(NewRow) && Map[NewRow].IsValidIndex(NewCol))
			{
				OutArray.Add(Map[NewRow][NewCol]);
			}
		}
	}
}

void UMapManager::TempSetStartLocation()
{
	int32 RandStartIsland = 50;
	GetWorld()->GetGameState<ASTSGameState>()->SetIslandOwner(50, GetWorld()->GetFirstPlayerController());

	Cast<APlayerCommander>(GetWorld()->GetFirstPlayerController()->GetPawn())->MoveCommanderToTile(IslandTiles[RandStartIsland][0], true);
}
