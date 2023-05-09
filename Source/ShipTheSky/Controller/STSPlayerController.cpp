// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/STSPlayerController.h"
#include "STSGameState.h"
#include "Pawn/Commander.h"
#include "Pawn/PlayerCommander.h"
#include "Tile/IslandTile.h"
#include "Building/BaseBuilding.h"
#include "Building/Barracks.h"
#include "Widget/IslandTileUI.h"
#include "Widget/ResourceUI.h"
#include "MapManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ASTSPlayerController::ASTSPlayerController()
{
	CameraMovementSpeed = 4000.0f;
}

bool ASTSPlayerController::OnButtonCreateUnitPressed(EUnitType Type)
{
	return Cast<ABarracks>(Commander->GetTargetIslandTile()->GetBuilding())->AddUnitCreationToArray(Type);
}

void ASTSPlayerController::OnButtonConstructBuildingPressed(EBuildingType Type)
{
	Commander->ConstructBuilding(Commander->GetTargetIslandTile(), Type);
}

void ASTSPlayerController::OnButtonGenerateMap()
{
	GetGameInstance()->GetSubsystem<UMapManager>()->ClearMap();
	GetGameInstance()->GetSubsystem<UMapManager>()->GenerateMap(75);
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
	IslandTileUI = Cast<UIslandTileUI>(CreateWidget<UUserWidget>(GetWorld(), IslandTileUIClass));
	if (IslandTileUI != nullptr)
	{
		IslandTileUI->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UI"));
	}
	ResourceUI = Cast<UResourceUI>(CreateWidget<UUserWidget>(GetWorld(), ResourceUIClass));
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
