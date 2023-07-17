// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/PlayerCommander.h"
#include "Controller/STSPlayerController.h"
#include "Tile/BaseTile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerCommander::APlayerCommander()
{
	SpringArmLength = 4000.0f;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = Root;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(Root);
	SpringArmComp->TargetArmLength = SpringArmLength;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	CommanderID = 0;
}

void APlayerCommander::MoveCommanderToTile(ABaseTile* Tile, bool bSelectTileToo)
{
	if (!Tile) return;
	SetActorLocation(Tile->GetActorLocation());
	if (bSelectTileToo)
	{
		if (TargetTile != Tile) Tile->OnTileSelectedAsView(Cast<ASTSPlayerController>(GetController()));
	}
}

void APlayerCommander::BeginPlay()
{
	Super::BeginPlay();
}
