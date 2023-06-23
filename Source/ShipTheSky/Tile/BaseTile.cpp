// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/BaseTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Ship.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABaseTile::ABaseTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->CastShadow = false;
}

void ABaseTile::TimePass(int32 GameDate)
{
}

// Called when the game starts or when spawned
void ABaseTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.94f, 0.975f, 0.975f));
}

void ABaseTile::OnTileSelectedAsView(ASTSPlayerController* PlayerController)
{
	if (PlayerController->GetIsPathSelectionMode() == false)
	{
		//�� �� �� ����
		StaticMeshComp->SetRenderCustomDepth(true);

		PlayerController->CloseOwningIslandUI();
		PlayerController->CloseShipUI();

		ABaseTile* CurrentTargetTile = PlayerController->GetCommander()->GetTargetTile();

		if (CurrentTargetTile != nullptr)
		{
			//�� �� �� ����
			CurrentTargetTile->GetStaticMeshComponent()->SetRenderCustomDepth(false);
			if (CurrentTargetTile == this) PlayerController->GetCommander()->SetTargetTile(nullptr);
			else
			{
				PlayerController->GetCommander()->SetTargetTile(this);
				if (ShipOnThisTile != nullptr) PlayerController->OpenShipUI();
			}
			PlayerController->GetCommander()->SetTargetShip(ShipOnThisTile);
		}
		else
		{
			PlayerController->GetCommander()->SetTargetTile(this);
			if (ShipOnThisTile != nullptr) PlayerController->OpenShipUI();
		}
	}
}

bool ABaseTile::OnTileFirstSelectedAsPath(ASTSPlayerController* PlayerController)
{
	if (PlayerController->GetIsPathSelectionMode())
	{
		if (PlayerController->GetCommander()->GetTargetShip() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Ship"));
			return false;
		}
		return PlayerController->GetCommander()->GetTargetShip()->TryAddTileToPath(this, true);
	}
	return false;
}

void ABaseTile::OnTileSelectedAsPath(ASTSPlayerController* PlayerController)
{
	if (PlayerController->GetIsPathSelectionMode())
	{
		if (PlayerController->GetCommander()->GetTargetShip() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Ship"));
			return;
		}
		PlayerController->GetCommander()->GetTargetShip()->TryAddTileToPath(this, false);
	}
}
