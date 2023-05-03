// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/IslandTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Unit/BaseUnit.h"

AIslandTile::AIslandTile()
{
	TileType = ETileType::Island;
	//Resources.Add(TPair<EResourceType, int32>(EResourceType::StoneSun, 5));
	//Resources.Add(TPair<EResourceType, int32>(EResourceType::WoodLightning, 4));
	//Resources.Add(TPair<EResourceType, int32>(EResourceType::Food, 3));
	//고쳐요 나중에
	Resource = FMath::RandRange(0, (int32)EResourceType::End - 1);
	Amount = FMath::RandRange(1, 4);
}

void AIslandTile::GiveTileEffect()
{
	if (UnitOnThisTile != nullptr)
	{
		ACommander* OwnerCommander = Cast<ACommander>(UnitOnThisTile->GetOwner());
		if (OwnerCommander != nullptr)
		{
			/*for (auto Resource : Resources)
			{
				OwnerCommander->SetResource(OwnerCommander->GetResource(Resource.Key) + Resource.Value, Resource.Key);
			}*/
			EResourceType Type = StaticCast<EResourceType>(Resource);
			OwnerCommander->SetResource(OwnerCommander->GetResource(Type) + Amount, Type);
		}
		else 
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong Owner"));
		}
	}
}

void AIslandTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void AIslandTile::OnTileReleased(AActor* Target, FKey ButtonPressed)
{
	ASTSPlayerController* PlayerController = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->GetCommander()->SetTargetIslandTile(this);
	PlayerController->SetIslandTileUIVisibility(true);
}