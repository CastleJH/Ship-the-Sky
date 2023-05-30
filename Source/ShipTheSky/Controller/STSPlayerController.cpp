// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/STSPlayerController.h"
#include "STSGameState.h"
#include "Pawn/Commander.h"
#include "Pawn/PlayerCommander.h"
#include "Tile/IslandTile.h"
#include "Tile/ResourceTile.h"
#include "Building/BaseBuilding.h"
#include "Building/Barracks.h"
#include "Building/Shipyard.h"
#include "MapManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Unit/BaseUnit.h"
#include "Ship.h"

ASTSPlayerController::ASTSPlayerController()
{
	CameraMovementSpeed = 4000.0f;
}

bool ASTSPlayerController::OnButtonCreateUnitPressed(EUnitType Type)
{
	return Cast<ABarracks>(Cast<AResourceTile>(Commander->GetTargetIslandTile())->GetBuilding())->AddUnitCreationToArray(Type);
}

bool ASTSPlayerController::OnButtonCreateShipPressed()
{
	return Cast<AShipyard>(Cast<AResourceTile>(Commander->GetTargetIslandTile())->GetBuilding())->AddShipCreationToArray();
}

void ASTSPlayerController::OnButtonConstructBuildingPressed(EBuildingType Type)
{
	Commander->ConstructBuilding(Cast<AResourceTile>(Commander->GetTargetIslandTile()), Type);
	OpenOwningIslandBuildingUI();
}

void ASTSPlayerController::OnButtonUnitEmbark(ABaseUnit* Unit)
{
	if (Commander->GetTargetIslandTile() != nullptr)
	{
		AShip* Ship = Commander->GetTargetIslandTile()->GetShip();
		if (Ship != nullptr)
		{
			Unit->Embark(Ship);
			UE_LOG(LogTemp, Warning, TEXT("Embark"));
		}
		else UE_LOG(LogTemp, Warning, TEXT("Embark Fail"));
	}
}

void ASTSPlayerController::OnButtonUnitDisembark(ABaseUnit* Unit)
{
	AIslandTile* IslandTile = Commander->GetTargetIslandTile();
	if (IslandTile != nullptr)
	{
		AShip* Ship = Commander->GetTargetIslandTile()->GetShip();
		Unit->Disembark(IslandTile->GetIslandID());
		UE_LOG(LogTemp, Warning, TEXT("Disembark"));
	}
}

void ASTSPlayerController::OnButtonGenerateMap()
{
	GetGameInstance()->GetSubsystem<UMapManager>()->ClearMap();
	GetGameInstance()->GetSubsystem<UMapManager>()->GenerateMap(60);
}

void ASTSPlayerController::CreateTileResourcesUIHolders(float LastXCoord, float LastYCoord)
{
	//return;
	HolderSize = 5000;
	HolderPadding = 500;
	TileResourcesUIHolderRow = (int32)LastXCoord / HolderSize + 1;
	TileResourcesUIHolderCol = (int32)LastYCoord / HolderSize + 1;
	for (int32 i = 0; i < TileResourcesUIHolderRow; i++)
	{
		for (int32 j = 0; j < TileResourcesUIHolderCol; j++)
		{
			AActor* Holder = GetWorld()->SpawnActor<AActor>(TileResourcesUIHolderClass, FVector(i * HolderSize, j * HolderSize, 180.0f), FRotator::ZeroRotator);
			TileResourcesUIHolders.Add(Holder);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Holder created"));
}

void ASTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveRight"), this, &ASTSPlayerController::MoveCameraHorizontal);
	InputComponent->BindAxis(TEXT("MoveForward"), this, &ASTSPlayerController::MoveCameraVertical);
	InputComponent->BindAxis(TEXT("Zoom"), this, &ASTSPlayerController::ZoomCamera);
}

void ASTSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Commander = Cast<ACommander>(GetPawn());
	IngameMainUI = CreateWidget<UUserWidget>(GetWorld(), IngameMainUIClass);
	if (IngameMainUI != nullptr)
	{
		IngameMainUI->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("Main UI"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UI"));
	}
	ResourceUI = CreateWidget<UUserWidget>(GetWorld(), ResourceUIClass);
	if (ResourceUI != nullptr)
	{
		ResourceUI->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UI"));
	}
}

void ASTSPlayerController::MoveCameraHorizontal(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.Y = Value * CameraMovementSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	GetPawn()->AddActorWorldOffset(DeltaLocation);
}

void ASTSPlayerController::MoveCameraVertical(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * CameraMovementSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	GetPawn()->AddActorWorldOffset(DeltaLocation);
}

void ASTSPlayerController::ZoomCamera(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.Z = Value * CameraMovementSpeed * UGameplayStatics::GetWorldDeltaSeconds(this) * 3.0f;
	GetPawn()->AddActorWorldOffset(DeltaLocation);
}
