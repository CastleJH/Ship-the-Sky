// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/STSPlayerController.h"
#include "STSGameState.h"
#include "Pawn/PlayerCommander.h"
#include "Tile/ResourceTile.h"
#include "Tile/GuardianTile.h"
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
#include "GameFramework/SpringArmComponent.h"

ASTSPlayerController::ASTSPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

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
	Commander->TryStopShip(Commander->GetTargetShip());
}

void ASTSPlayerController::OnButtonLookTile(class ABaseTile* Tile)
{
	PlayerCommander->MoveCommanderToTile(Tile, true);
}

void ASTSPlayerController::OnButtonLocateUnitOnTile(ABaseUnit* Unit)
{
	UnitWaitingRelocationFromUI = Unit;
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
	EnhancedInputComponent->BindAction(InputMousePressedForReloc, ETriggerEvent::Triggered, this, &ASTSPlayerController::MousePressedForReloc);
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
	PlayerCommander->SpringArmComp->TargetArmLength += CameraZoomSpeed * Zoom;
}

void ASTSPlayerController::MousePressedForReloc(const FInputActionValue& Value)
{
	ABaseTile* Tile = MouseRay();
	if (Tile != nullptr)
	{
		FirstTile = Cast<AIslandTile>(Tile);
	}
	UE_LOG(LogTemp, Warning, TEXT("Pressed"));
}

void ASTSPlayerController::MouseReleased(const FInputActionValue& Value)
{
	ABaseTile* Tile = MouseRay();
	if (Tile != nullptr)
	{
		Tile->OnTileSelectedAsView(this);
		LockedShip = nullptr;

		AIslandTile* SecondTile = Cast<AIslandTile>(Tile);
		if (FirstTile && SecondTile && FirstTile != SecondTile)
		{
			RelocateUnitOnTwoTile(FirstTile, SecondTile);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Released"));
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

void ASTSPlayerController::RelocateUnitOnTwoTile(AIslandTile* Tile1, AIslandTile* Tile2)
{
	ABaseUnit* Unit1 = nullptr;
	AResourceTile* ResourceTile1 = Cast<AResourceTile>(Tile1);
	if (ResourceTile1 != nullptr) Unit1 = ResourceTile1->GetUnit();

	ABaseUnit* Unit2 = nullptr;
	AResourceTile* ResourceTile2 = Cast<AResourceTile>(Tile2);
	if (ResourceTile2 != nullptr) Unit2 = ResourceTile2->GetUnit();

	if (Unit1 != nullptr) Unit1->LocateOnIslandTile(Tile2, false);
	if (Unit2 != nullptr) Unit2->LocateOnIslandTile(Tile1, false);
}

void ASTSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Commander = Cast<ACommander>(GetPawn());
	PlayerCommander = Cast<APlayerCommander>(Commander);
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
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ReturnTile->GetName());
		return ReturnTile;
	}
	return nullptr;
}

void ASTSPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LockedShip != nullptr)
	{
		if (Commander->GetTargetTile() != LockedShip->GetCurTile()) LockedShip->GetCurTile()->OnTileSelectedAsView(this);
		Commander->SetActorLocation(LockedShip->GetActorLocation());
	}

	if (Commander->GetTargetShip() != PrevShip)
	{
		if (PrevShip)
		{
			PrevShip->bIsBeingObserved = false;
			PrevShip->ClearPathUI();
		}
		PrevShip = Commander->GetTargetShip();
		if (Commander->GetTargetShip())
		{
			Commander->GetTargetShip()->bIsBeingObserved = true;
		}
	}

	if (Commander->GetTargetShip())
	{
		Commander->GetTargetShip()->UpdatePathUI();
	}

	if (UnitWaitingRelocationFromUI)
	{
		AIslandTile* IslandTile = Cast<AIslandTile>(MouseRay());
		if (IslandTile)
		{
			RelocateUnitOnTwoTile(UnitWaitingRelocationFromUI->GetCurIslandTile(), IslandTile);
		}
		UnitWaitingRelocationFromUI = nullptr;
	}
}
