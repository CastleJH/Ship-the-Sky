// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "STSGameState.h"
#include "Tile/IslandTile.h"
#include "Tile/ResourceTile.h"
#include "Tile/GuardianTile.h"
#include "Pawn/PlayerCommander.h"
#include "Controller/STSPlayerController.h"
#include "Enums.h"
#include "Ship.h"
#include "Tile/BaseTile.h"
#include "Util/IndexPriorityQueue.h"
#include "Components/WidgetComponent.h"

UMapManager::UMapManager()
{
	NumColOfTheMap = 40;
	WorldEnemyWeight = 1.0f;
	EndYear = 2250;
}

void UMapManager::GenerateMap()
{
	if (!GetWorld()->GetGameState<ASTSGameState>())
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong stsgamestate."));
		return;
	}
	GetWorld()->GetGameState<ASTSGameState>()->EndYear = EndYear;

	Map.Empty();
	IslandTiles.Empty();
	NearestIslands.Empty();
	ResourceWidgetComps.Empty();

	int32 NumCol = NumColOfTheMap;
	int32 NumRow = FMath::RoundToInt((float)NumCol * 0.75f);
	int32 CurR, CurC, NewR, NewC, MaxPoss;
	int32 NewIslandID = 0;
	int32 Iter = 8;
	ETileType NewType, NearType;

	TArray<TArray<ETileType>> MapCalc;
	TArray<TArray<ETileType>> MapData;
	TArray<TArray<int32>> IslandID;

	if (NumRow < 25 || NumRow > 80)
	{
		UE_LOG(LogTemp, Error, TEXT("Too small/big map size."));
		return;
	}

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

	//시작섬 생성

	NewIslandID = 0;

	int32 CommanderCount = GetWorld()->GetGameState<ASTSGameState>()->Commanders.Num();

	//시작 위치들 조정
	if (CommanderCount != 5) {
		UE_LOG(LogTemp, Error, TEXT("Number of Commanders is not 5."));
		return;
	}

	GetWorld()->GetGameState<ASTSGameState>()->Commanders[0]->StartRow = NumRow / 2;
	GetWorld()->GetGameState<ASTSGameState>()->Commanders[0]->StartCol = NumCol / 2;

	GetWorld()->GetGameState<ASTSGameState>()->Commanders[1]->StartRow = NumRow - 5;
	GetWorld()->GetGameState<ASTSGameState>()->Commanders[1]->StartCol = 5;

	GetWorld()->GetGameState<ASTSGameState>()->Commanders[2]->StartRow = 5;
	GetWorld()->GetGameState<ASTSGameState>()->Commanders[2]->StartCol = 5;

	GetWorld()->GetGameState<ASTSGameState>()->Commanders[3]->StartRow = 5;
	GetWorld()->GetGameState<ASTSGameState>()->Commanders[3]->StartCol = NumCol - 5;

	GetWorld()->GetGameState<ASTSGameState>()->Commanders[4]->StartRow = NumRow - 5;
	GetWorld()->GetGameState<ASTSGameState>()->Commanders[4]->StartCol = NumCol - 5;

	for (auto Commander : GetWorld()->GetGameState<ASTSGameState>()->Commanders)
	{
		if (!MapData.IsValidIndex(Commander->StartRow) || !MapData[Commander->StartRow].IsValidIndex(Commander->StartCol))
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Start Island Index"));
			return;
		}
		if (MapData[Commander->StartRow][Commander->StartCol] == ETileType::Island)
		{
			UE_LOG(LogTemp, Warning, TEXT("Same Start Island Index"));
			return;
		}
		IslandID[Commander->StartRow][Commander->StartCol] = NewIslandID + 10000;
		MapData[Commander->StartRow][Commander->StartCol] = ETileType::Island;
		NewIslandID++;
		TArray<AIslandTile*> SameIslands;
		IslandTiles.Add(SameIslands);

		int32 IslandTileNum = 0;
		for (int32 Near = 0; Near < 6; Near++)
		{
			if (Near == Commander->EmptyStartIndex) continue;
			NewR = Commander->StartRow + RowOffset[Near];
			NewC = Commander->StartCol + ColOffset[Commander->StartRow % 2][Near];

			if (!MapData.IsValidIndex(NewR) || !MapData[NewR].IsValidIndex(NewC))
			{
				UE_LOG(LogTemp, Warning, TEXT("Start Island Index is too far from center."));
				return;
			}
			IslandID[NewR][NewC] = NewIslandID;
			MapData[NewR][NewC] = ETileType::Island;
		}
	}

	//섬 생성
	for (int32 Col = 0; Col < NumCol; Col++)
	{
		int32 IterIsland = NumCol > 70 ? 3 : 2;
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
				maxIter--;
			} while (NearCheck && maxIter);

			if (NearCheck) {
				//UE_LOG(LogTemp, Error, TEXT("Wrong Map Generation"));
				//return;
				continue;
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

	//수호자 타일과 시작 섬 먼저 스폰
	float Offs = 500.f;
	float XCoord = 0.0f, YCoord = 0.0f;
	float XDiff = Offs * 0.25f + Offs / FMath::Sqrt(3.0f);
	float YDiff = Offs;
	CurR = 0;
	Map.SetNum(MapData.Num());
	for (int32 Ridx = 0; Ridx < Map.Num(); Ridx++)
	{
		TArray<ABaseTile*> MapRow;
		MapRow.SetNum(NumCol);
		Map[Ridx] = MapRow;
	}
	for (auto Row : MapData)
	{
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
					Map[CurR][CurC] = IslandTile;
					IslandTile->SetRow(CurR);
					IslandTile->SetCol(CurC);

					//시작 섬인경우
					if (GuardianTileID < GetWorld()->GetGameState<ASTSGameState>()->Commanders.Num())
					{
						int32 GenNum = 0;
						for (int32 Near = 0; Near < 6; Near++)
						{
							if (Near == GetWorld()->GetGameState<ASTSGameState>()->Commanders[GuardianTileID]->EmptyStartIndex) continue;
							NewR = GetWorld()->GetGameState<ASTSGameState>()->Commanders[GuardianTileID]->StartRow + RowOffset[Near];
							NewC = GetWorld()->GetGameState<ASTSGameState>()->Commanders[GuardianTileID]->StartCol + ColOffset[GetWorld()->GetGameState<ASTSGameState>()->Commanders[GuardianTileID]->StartRow % 2][Near];

							if (!MapData.IsValidIndex(NewR) || !MapData[NewR].IsValidIndex(NewC))
							{
								UE_LOG(LogTemp, Warning, TEXT("Wrong Island"));
								return;
							}
							//FVector StartLocation(XCoord + (float)RowOffset[Near] * XDiff, YCoord + (float)ColOffset[NewR % 2][Near] * YDiff + ((NewR & 1) ? Offs * 0.5f : 0.0f), 60.0f);
							FVector StartLocation(XCoord + (float)RowOffset[Near] * XDiff, YCoord + (float)ColOffset[NewR % 2][Near] * YDiff + ((NewR & 1) ? Offs * 0.5f : 0.0f) + (CurR & 1 ? Offs * -0.5f : 0.0f), 60.0f);
							TMap<uint8, float> StartResources;
							switch (GenNum)
							{
							case 0:
								IslandTile = GetWorld()->SpawnActor<AIslandTile>(MineTileClass, StartLocation, FRotator::ZeroRotator);
								StartResources.Add(0, 5);
								StartResources.Add(1, 5);
								StartResources.Add(2, 5);
								break;
							case 1:
								IslandTile = GetWorld()->SpawnActor<AIslandTile>(MineTileClass, StartLocation, FRotator::ZeroRotator);
								StartResources.Add(3, 5);
								StartResources.Add(4, 5);
								break;
							case 2:
								IslandTile = GetWorld()->SpawnActor<AIslandTile>(ForestTileClass, StartLocation, FRotator::ZeroRotator);
								StartResources.Add(5, 5);
								StartResources.Add(6, 5);
								StartResources.Add(7, 5);
								break;
							case 3:
								IslandTile = GetWorld()->SpawnActor<AIslandTile>(ForestTileClass, StartLocation, FRotator::ZeroRotator);
								StartResources.Add(8, 5);
								StartResources.Add(9, 5);
								break;
							case 4:
								IslandTile = GetWorld()->SpawnActor<AIslandTile>(FarmTileClass, StartLocation, FRotator::ZeroRotator);
								StartResources.Add(10, 8);
								break;
							}
							IslandTile->SetIslandID(GuardianTileID);
							Cast<AResourceTile>(IslandTile)->SetResources(StartResources);
							Cast<AResourceTile>(IslandTile)->WorldEnemyWeight = WorldEnemyWeight;
							ResourceWidgetComps.Add(Cast<AResourceTile>(IslandTile)->GetWidgetComponent());
							Map[NewR][NewC] = IslandTile;
							IslandTile->SetRow(NewR);
							IslandTile->SetCol(NewC);
							IslandTiles[GuardianTileID].Add(IslandTile);
							GenNum++;
						}
					}
				}
			}
			YCoord += YDiff;
			CurC++;
		}
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
			if (Map[CurR][CurC] == nullptr)
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
						IslandTile->SetIslandID(IslandID[CurR][CurC]);
						Cast<AResourceTile>(IslandTile)->WorldEnemyWeight = WorldEnemyWeight;
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
			}
			YCoord += YDiff;
			CurC++;
		}
		XCoord += XDiff;
		CurR++;
	}
	XCoord -= XDiff;
	YCoord -= YDiff;

	ASTSPlayerController* Controller = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (Controller->GuardianClass.Num() != 10)
	{
		UE_LOG(LogTemp, Error, TEXT("Guardian must be 10 types."));
		return;
	}
	int32 PlayerIslandCount = 0;
	int32 GuardianCount[10] = { 0 };
	int32 MaximumGuardianCount = NewIslandID / 10 + 1;
	GuardianCount[9] = -4;
	for (auto Row : IslandTiles)
	{
		if (Row.IsValidIndex(0))
		{
			AGuardianTile* GuardianTile = Cast<AGuardianTile>(Row[0]);
			if (GuardianTile)
			{
				if (PlayerIslandCount < GetWorld()->GetGameState<ASTSGameState>()->Commanders.Num())
				{
					GuardianTile->SpawnGuardian(Controller->GuardianClass.Num() - 1);
					GuardianCount[9]++;
				}
				else
				{
					int32 RandGuardianType = -1;
					do 
					{ 
						RandGuardianType = FMath::RandRange(0, Controller->GuardianClass.Num() - 1);
					} 
					while (GuardianCount[RandGuardianType] >= MaximumGuardianCount);
					GuardianCount[RandGuardianType]++;
					GuardianTile->SpawnGuardian(RandGuardianType);
				}
				TArray<ABaseTile*> InTile;
				GetAdjacentTiles(GuardianTile, InTile);
				GuardianTile->SetAdjTiles(InTile);
				TArray<AResourceTile*> ResourceInTile;
				for (auto Elem : InTile)
				{
					AResourceTile* ResourceTile = Cast<AResourceTile>(Elem);
					if (ResourceTile) ResourceInTile.Add(ResourceTile);
				}
				GuardianTile->SetAdjResourceTiles(ResourceInTile);
			}
		}
		PlayerIslandCount++;
	}
	//20
	GetWorld()->GetGameState<ASTSGameState>()->ResetIslandOwner(NewIslandID, 0, false);

	SetTilePowers();
	SetIslandResources();
	SetStartLocation();

	for (auto Row : Map)
	{
		for (auto Tile : Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *(Tile->GetName()));
		}
	}

	
	for (int32 i = 0; i < IslandTiles.Num(); i++)
	{
		TArray<TPair<int32, AGuardianTile*>> Row;
		for (int32 j = 0; j < IslandTiles.Num(); j++)
		{
			if (i == j) continue;
			Row.Add({ GetDistanceOfTwoTile(IslandTiles[i][0], IslandTiles[j][0]), Cast<AGuardianTile>(IslandTiles[j][0]) });
		}
		Row.Sort([](const TPair<int32, AGuardianTile*>& A, const TPair<int32, AGuardianTile*>& B){ return A.Key < B.Key; });
		NearestIslands.Add(Row);
	}
}

void UMapManager::TimePassesToAllTile(int32 GameDate)
{
	for (auto Tiles : Map)
	{
		for (auto Tile : Tiles)
		{
			Tile->TimePass(GameDate);
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

bool UMapManager::MakePathToTile(AShip* Ship, ABaseTile* EndTile, bool bIsForBattle)
{
	if (!Ship || !EndTile || !Ship->GetCurTile())
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Status of ship"));
		return false;
	}

	//TPair<int32, ABaseTile*>
	TArray<PQElem> Opened;
	TArray<TArray<bool>> Closed;
	TArray<TArray<Cell>> Cells;

	int32 INF = 1987654321;
	int32 CurRow;
	int32 CurCol;
	int32 NewRow;
	int32 NewCol;
	int32 Ng;
	int32 Nh;
	int32 Nf;

	bool FoundPath = false;

	if (bIsForBattle)
	{
		int32 MinDist = 1987654321;
		int32 TmpDist = 0;
		AGuardianTile* g = Cast<AGuardianTile>(EndTile);
		if (!g) 
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong endtile for battle"));
			return false;
		}
		EndTile = nullptr;
		for (auto Tile : g->GetAdjTiles())
		{
			if (Tile->GetTileType() != ETileType::Island && !Tile->GetShip())
			{
				if (!EndTile)
				{
					EndTile = Tile;
					MinDist = GetDistanceOfTwoTile(Ship->GetCurTile(), Tile);
				}
				else 
				{
					TmpDist = GetDistanceOfTwoTile(Ship->GetCurTile(), Tile);
					if (TmpDist < MinDist)
					{
						MinDist = TmpDist;
						EndTile = Tile;
					}
				}
			}
		}
	}

	if (!EndTile) return false;

	//초기화
	for (int32 i = 0; i < Map.Num(); i++)
	{
		TArray<bool> Row;
		Row.Init(false, Map[i].Num());
		Closed.Add(Row);

		TArray<Cell> CRow;
		Cell C;
		CRow.Init(C, Map[i].Num());
		for (int32 j = 0; j < Map[i].Num(); j++)
		{
			CRow[j].Tile = Map[i][j];
			CRow[j].Parent = nullptr;
			CRow[j].f = INF;
			CRow[j].g = INF;
		}
		Cells.Add(CRow);
	}

	//첫 좌표 표시
	Cells[Ship->GetCurTile()->GetRow()][Ship->GetCurTile()->GetCol()].f = 0;
	Cells[Ship->GetCurTile()->GetRow()][Ship->GetCurTile()->GetCol()].g = 0;

	Cells[Ship->GetCurTile()->GetRow()][Ship->GetCurTile()->GetCol()].Parent = Ship->GetCurTile();
	Opened.Add(PQElem{ 0, Ship->GetCurTile() });

	UE_LOG(LogTemp, Warning, TEXT(".."));
	while (!Opened.IsEmpty())
	{
		PQElem Elem;
		Opened.HeapPop(Elem);

		CurRow = Elem.Tile->GetRow();
		CurCol = Elem.Tile->GetCol();
		Closed[CurRow][CurCol] = true;

		for (int32 i = 0; i < 6; i++)
		{
			NewRow = CurRow + RowOffset[i];
			NewCol = CurCol + ColOffset[CurRow % 2][i];
			if (Map.IsValidIndex(NewRow) && Map[NewRow].IsValidIndex(NewCol))
			{
				if (NewRow == EndTile->GetRow() && NewCol == EndTile->GetCol())
				{
					Cells[NewRow][NewCol].Parent = Elem.Tile;
					FoundPath = true; 
					break;
				}
				else if (!Closed[NewRow][NewCol] && GetIsTileAccessible(Ship->GetOwnerCommander(), Map[NewRow][NewCol]))
				{
					Ng = Cells[CurRow][CurCol].g + 1;
					Nh = GetDistanceOfTwoTile(Map[NewRow][NewCol], EndTile);
					Nf = Ng + Nh;

					if (Cells[NewRow][NewCol].f > Nf)
					{
						Cells[NewRow][NewCol].f = Nf;
						Cells[NewRow][NewCol].g = Ng;
						Cells[NewRow][NewCol].Parent = Elem.Tile;
						Opened.HeapPush(PQElem{ Nf, Map[NewRow][NewCol] });
					}
				}
			}
		}
		if (FoundPath) break;
	}

	if (FoundPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Path"));
		TArray<Cell> Stack;
		Stack.Push(Cells[EndTile->GetRow()][EndTile->GetCol()]);

		while (Stack.Top().Parent != Stack.Top().Tile)
		{
			Stack.Push(Cells[Stack.Top().Parent->GetRow()][Stack.Top().Parent->GetCol()]);
		}

		Cell C = Stack.Pop();
		Ship->TryAddTileToPath(C.Tile, true);
		while (!Stack.IsEmpty())
		{
			C = Stack.Pop();
			if (!Ship->TryAddTileToPath(C.Tile, false)) return false;
		}
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Path: %s"), *Ship->GetName());
		return false;
	}

}

int32 UMapManager::GetDistanceOfTwoTile(ABaseTile* Tile1, ABaseTile* Tile2)
{
	int32 Dr = FMath::Abs(Tile1->GetRow() - Tile2->GetRow());
	int32 Dc = FMath::Abs(Tile1->GetCol() - Tile2->GetCol());

	return Dr + Dc - (Dr + (Tile1->GetRow() & 1 ? 0 : 1)) / 2;
}

//배가 없으면서, 적 섬타일이 아니면서, 적대적인 수호자의 인접타일이 아닌지 반환
bool UMapManager::GetIsTileAccessible(ACommander* Commander, ABaseTile* Tile)
{
	if (Tile->GetShip()) return false;

	if (Tile->GetTileType() == ETileType::Island)
	{
		if (Cast<AGuardianTile>(Tile)) return false;
		if (Cast<AIslandTile>(Tile)->GetIslandOwner() == Commander) return true;
		return false;
	}
	else
	{
		TArray<ABaseTile*> AdjTiles;
		GetAdjacentTiles(Tile, AdjTiles);
		for (auto Adj : AdjTiles)
		{
			AGuardianTile* GuardianTile = Cast<AGuardianTile>(Adj);
			if (GuardianTile && GuardianTile->GetIslandOwner() != Commander) return false;
		}
		return true;
	}
}

bool UMapManager::SetResoureUIVisibility(bool bIsVisible)
{
	ESlateVisibility Vis = bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
	for (auto WidgetComp : ResourceWidgetComps)
	{
		WidgetComp->GetWidget()->SetVisibility(Vis);
	}

	return false;
}

FVector UMapManager::GetMaxCameraLocation() const
{
	return Map.Last().Last()->GetActorLocation();
}

float UMapManager::GetMaxCameraSpringArm() const
{
	return (float)Map[0].Num() * 340.0f;
}

void UMapManager::SetTilePowers()
{
	int32 NewR;
	int32 NewC;
	for (int32 Row = 0; Row < Map.Num(); Row++)
	{
		for (int32 Col = 0; Col < Map[Row].Num(); Col++)
		{
			int32 Count = 0;
			for (int32 i = 0; i < 18; i++)
			{
				NewR = Row + RowOffset[i];
				NewC = Col + ColOffset[Row % 2][i];

				if (Map.IsValidIndex(NewR) && Map[NewR].IsValidIndex(NewC))
				{
					if (Map[NewR][NewC]->GetTileType() == Map[Row][Col]->GetTileType())
					{
						Count++;
					}
				}
			}
			if (Count <= 8) Map[Row][Col]->SetTilePower(1);
			else if (Count <= 11) Map[Row][Col]->SetTilePower(2);
			else if (Count <= 14) Map[Row][Col]->SetTilePower(3);
			else Map[Row][Col]->SetTilePower(4);
		}
	}
}

void UMapManager::SetIslandResources()
{
	int32 NewR;
	int32 NewC;

	ETileType MostTile = ETileType::Cloud;
	float TilePower;
	int32 MaxCount[6] = { 0 };

	int32 MinCount = 0;
	int32 ResourceCount[11] = { 0 };
	
	int32 Skip = 0;
	for (auto Row : IslandTiles)
	{
		if (Skip < GetWorld()->GetGameState<ASTSGameState>()->Commanders.Num())
		{
			Skip++;
			continue;
		}
		for (int32 Idx = 1; Idx < Row.Num(); Idx++)
		{
			TilePower = 0;
			MaxCount[0] = 0; 
			MaxCount[1] = 0; 
			MaxCount[2] = 0; 
			MaxCount[3] = 0; 
			MaxCount[4] = 0; 
			MaxCount[5] = 0;
			MostTile = ETileType::Cloud;
			for (int32 i = 0; i < 18; i++)
			{
				NewR = Row[Idx]->GetRow() + RowOffset[i];
				NewC = Row[Idx]->GetCol() + ColOffset[Row[Idx]->GetRow() % 2][i];
				if (Map.IsValidIndex(NewR) && Map[NewR].IsValidIndex(NewC))
				{
					MaxCount[(int32)Map[NewR][NewC]->GetTileType()]++;
					if (Map[NewR][NewC]->GetTileType() != ETileType::Island && MaxCount[(uint8)MostTile] <= MaxCount[(int32)Map[NewR][NewC]->GetTileType()])
					{
						if (MostTile == Map[NewR][NewC]->GetTileType())
						{
							float tmp = TilePower;
							TilePower = FMath::Max(TilePower, Map[NewR][NewC]->GetTilePower());
							if (tmp != TilePower) Row[Idx]->AffectedTile = Map[NewR][NewC];
						}
						else
						{
							MostTile = Map[NewR][NewC]->GetTileType();
							TilePower = Map[NewR][NewC]->GetTilePower();
							Row[Idx]->AffectedTile = Map[NewR][NewC];

						}
					}
				}
			}

			TMap<uint8, float> Resources;
			int32 Additional = FMath::RandRange(0, 2);
			int32 Amount;
			uint8 ResourceType;
			switch (Row[Idx]->GetIslandType())
			{
			case EIslandTileType::Mine:
				ResourceType = (uint8)MostTile - 1;
				Amount = FMath::RandRange(1, 3) + TilePower;
				Resources.Add(ResourceType, Amount);
				ResourceCount[ResourceType] += Amount;

				MinCount = 999999;
				for (int32 Type = (int32)EResourceType::StoneCloud; Type <= (int32)EResourceType::StoneMeteor; Type++)
				{
					if (ResourceCount[Type] < MinCount)
					{
						MinCount = ResourceCount[Type];
						ResourceType = Type;
					}
				}

				for (; Additional != 0; Additional--)
				{
					Amount = FMath::RandRange(1, 3);
					Resources.Add(ResourceType, Amount);
					ResourceCount[ResourceType] += Amount;

					MinCount = 999999;
					for (int32 Type = (int32)EResourceType::StoneCloud; Type <= (int32)EResourceType::StoneMeteor; Type++)
					{
						if (ResourceCount[Type] < MinCount)
						{
							MinCount = ResourceCount[Type];
							ResourceType = Type;
						}
					}
				}
				break;
			case EIslandTileType::Forest:
				ResourceType = (uint8)MostTile + 4;
				Amount = FMath::RandRange(1, 3) + TilePower;
				Resources.Add(ResourceType, Amount);
				ResourceCount[ResourceType] += Amount;

				MinCount = 999999;
				for (int32 Type = (int32)EResourceType::WoodCloud; Type <= (int32)EResourceType::WoodMeteor; Type++)
				{
					if (ResourceCount[Type] < MinCount)
					{
						MinCount = ResourceCount[Type];
						ResourceType = Type;
					}
				}

				for (; Additional != 0; Additional--)
				{
					Amount = FMath::RandRange(1, 3);
					Resources.Add(ResourceType, Amount);
					ResourceCount[ResourceType] += Amount;

					MinCount = 999999;
					for (int32 Type = (int32)EResourceType::WoodCloud; Type <= (int32)EResourceType::WoodMeteor; Type++)
					{
						if (ResourceCount[Type] < MinCount)
						{
							MinCount = ResourceCount[Type];
							ResourceType = Type;
						}
					}
				}
				break;
			case EIslandTileType::Farm:
				Resources.Add((uint8)EResourceType::Food, FMath::RandRange(1, 3) + TilePower);
				break;
			case EIslandTileType::Guardian:
				break;
			default:
				break;
			}
			if (Row[Idx]->GetIslandType() != EIslandTileType::Guardian)
			{
				Cast<AResourceTile>(Row[Idx])->SetResources(Resources);
				ResourceWidgetComps.Add(Cast<AResourceTile>(Row[Idx])->GetWidgetComponent());
			}
		}
	}
}

void UMapManager::SetStartLocation()
{
	ASTSGameState* GameState = GetWorld()->GetGameState<ASTSGameState>();
	int32 IslandID = 0;
	for (auto Commander : GameState->Commanders)
	{
		GameState->SetIslandOwner(IslandID, Commander);
		IslandID++;
	}
	SelectAllIslandTiles();
	Cast<APlayerCommander>(GetWorld()->GetFirstPlayerController()->GetPawn())->MoveCommanderToTile(IslandTiles[0][0], true);
}

void UMapManager::SelectAllIslandTiles()
{
	ASTSPlayerController* Controller = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());
	for (auto elem : IslandTiles)
	{
		for (auto Tile : elem)
		{
			Tile->OnTileSelectedAsView(Controller);
		}
	}
}
