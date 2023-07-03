// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/STSPlayerController.h"
#include "STSGameState.h"
#include "Pawn/PlayerCommander.h"
#include "Tile/ResourceTile.h"
#include "Building/Barracks.h"
#include "Building/Shipyard.h"
#include "MapManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Unit/BaseUnit.h"
#include "Ship.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ASTSPlayerController::ASTSPlayerController()
{
	CameraMovementSpeed = 125.0f;
	CameraZoomSpeed = 400.0f;
	bIsPathSelectionMode = false;
	bIsPathSelectionValid = false;
}

bool ASTSPlayerController::OnButtonCreateUnitPressed(EUnitType Type)
{
	if (Commander->GetTargetIslandTile() == nullptr) return false;
	return Commander->TryCreateUnit(Cast<ABarracks>(Commander->GetTargetResoureTile()->GetBuilding()), Type);
}

bool ASTSPlayerController::OnButtonCreateShipPressed()
{
	if (Commander->GetTargetIslandTile() == nullptr) return false;
	return Commander->TryCreateShip(Cast<AShipyard>(Commander->GetTargetResoureTile()->GetBuilding()));
}

void ASTSPlayerController::OnButtonConstructBuildingPressed(EBuildingType Type)
{
	Commander->ConstructBuilding(Cast<AResourceTile>(Commander->GetTargetIslandTile()), Type);
}

void ASTSPlayerController::OnButtonUnitEmbark(ABaseUnit* Unit)
{
	Commander->TryEmbarkUnit(Commander->GetTargetShip(), Unit);
}

void ASTSPlayerController::OnButtonUnitDisembark(ABaseUnit* Unit)
{
	Commander->TryDisembarkUnit(Unit);
}

void ASTSPlayerController::OnButtonDepartShip()
{
	Commander->TryDepartShip(Commander->GetTargetShip());
	SetIsPathSelectionMode(false);
}

void ASTSPlayerController::OnButtonStopShip()
{
}

void ASTSPlayerController::SetIsPathSelectionMode(bool IsPathSelectionMode)
{
	bIsPathSelectionMode = IsPathSelectionMode;
	UInputMappingContext* NewMappingContext = PathSelectionMode;
	if (bIsPathSelectionMode)
	{
		NewMappingContext = PathSelectionMode;
	}
	else
	{
		NewMappingContext = TileSelectionMode;
	}


	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(NewMappingContext, 0);
	}
}

void ASTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong InputComponent"));
	}
	EnhancedInputComponent->BindAction(InputMove, ETriggerEvent::Triggered, this, &ASTSPlayerController::MoveCamera);
	EnhancedInputComponent->BindAction(InputZoom, ETriggerEvent::Triggered, this, &ASTSPlayerController::ZoomCamera);
	EnhancedInputComponent->BindAction(InputMouseReleased, ETriggerEvent::Triggered, this, &ASTSPlayerController::MouseReleased);
	EnhancedInputComponent->BindAction(InputMousePressedForPath, ETriggerEvent::Triggered, this, &ASTSPlayerController::MousePressedForPath);
	EnhancedInputComponent->BindAction(InputMouseDraggedForPath, ETriggerEvent::Triggered, this, &ASTSPlayerController::MouseDraggedForPath);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = TileSelectionMode;
		Subsystem->AddMappingContext(NewMappingContext, 0);
	}
}

void ASTSPlayerController::MoveCamera(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	Commander->AddActorWorldOffset(FVector(MovementVector.X, MovementVector.Y, 0.0f) * CameraMovementSpeed);
}

void ASTSPlayerController::ZoomCamera(const FInputActionValue& Value)
{
	float Zoom = Value.Get<float>();
	Commander->AddActorWorldOffset(FVector(0.0f, 0.0f, Zoom) * CameraZoomSpeed);
}

void ASTSPlayerController::MouseReleased(const FInputActionValue& Value)
{
	ABaseTile* Tile = MouseRay();
	if (Tile != nullptr) Tile->OnTileSelectedAsView(this);
}

void ASTSPlayerController::MousePressedForPath(const FInputActionValue& Value)
{
	ABaseTile* Tile = MouseRay();
	if (Tile != nullptr) bIsPathSelectionValid = Tile->OnTileFirstSelectedAsPath(this);
}

void ASTSPlayerController::MouseDraggedForPath(const FInputActionValue& Value)
{
	if (!bIsPathSelectionValid) return;
	ABaseTile* Tile = MouseRay();
	if (Tile != nullptr) Tile->OnTileSelectedAsPath(this);
}

void ASTSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Commander = Cast<ACommander>(GetPawn());
	IngameUI = CreateWidget<UUserWidget>(GetWorld(), IngameUIClass);
	if (IngameUI != nullptr)
	{
		IngameUI->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("Main UI"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UI"));
	}
}

ABaseTile* ASTSPlayerController::MouseRay()
{
	FHitResult OutResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, OutResult);
	if (OutResult.bBlockingHit)
	{
		ABaseTile* ReturnTile = Cast<ABaseTile>(OutResult.GetActor());
		return ReturnTile;
	}
	return nullptr;
}
