// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/BaseTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"

// Sets default values
ABaseTile::ABaseTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->CastShadow = false;
	OnReleased.AddDynamic(this, &ABaseTile::OnTileReleased);
}

void ABaseTile::TimePass(int32 GameDate)
{
}

// Called when the game starts or when spawned
void ABaseTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.94f, 0.975f, 0.975f));
	//SetActorScale3D(FVector(0.893f, 0.92625f, 0.92625f));
	//SetActorScale3D(FVector(0.846f, 0.8775f, 0.8775f));
}

void ABaseTile::OnTileReleased(AActor* Target, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());

	ASTSPlayerController* PlayerController = Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController());
	StaticMeshComp->SetRenderCustomDepth(true);
	PlayerController->CloseOwningIslandUI();
	PlayerController->CloseShipUI();

	ABaseTile* CurrentTargetTile = PlayerController->GetCommander()->GetTargetTile();

	if (CurrentTargetTile != nullptr)
	{
		CurrentTargetTile->GetStaticMeshComponent()->SetRenderCustomDepth(false);
		if (CurrentTargetTile == this) PlayerController->GetCommander()->SetTargetTile(nullptr);
		else
		{
			PlayerController->GetCommander()->SetTargetTile(this);
			if (ShipOnThisTile != nullptr) PlayerController->OpenShipUI();
		}
	}
	else
	{
		PlayerController->GetCommander()->SetTargetTile(this);
		if (ShipOnThisTile != nullptr) PlayerController->OpenShipUI();
	}
}