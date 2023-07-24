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
#include "Building/Portal.h"

ASTSPlayerController::ASTSPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraMovementSpeed = 125.0f;
	CameraZoomSpeed = 400.0f;
	UserInputMode = EUserInputMode::View;
	bIsPathSelectionValid = false;
	bIsUnitRelocationMode = false;

	RemovingShipTarget = nullptr;
	PrevArmLength = 60.0f;
}

bool ASTSPlayerController::OnButtonCreateUnitPressed(EUnitType Type)
{
	if (Commander->GetTargetIslandTile() == nullptr) return false;
	return Commander->TryCreateUnit(Cast<ABarracks>(Commander->GetTargetResoureTile()->GetBuilding()), Type);
}

void ASTSPlayerController::SetToViewMode()
{
	UserInputMode = EUserInputMode::View;
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(ViewTileMode, 0);
	}
}

void ASTSPlayerController::SetToPathSelectionMode()
{
	UserInputMode = EUserInputMode::PathSelection;
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(PathSelectionMode, 0);
	}
}

void ASTSPlayerController::SetToPortalSelectionMode()
{
	UserInputMode = EUserInputMode::PortalSelecton;
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(PortalSelectionMode, 0);
	}
}

bool ASTSPlayerController::OnButtonCreateShipPressed()
{
	if (Commander->GetTargetIslandTile() == nullptr) return false;
	return Commander->TryCreateShip(Cast<AShipyard>(Commander->GetTargetResoureTile()->GetBuilding()));
}

void ASTSPlayerController::OnButtonConstructBuildingPressed(EBuildingType Type)
{
	Commander->TryConstructBuilding(Cast<AResourceTile>(Commander->GetTargetIslandTile()), Type);
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
	SetToViewMode();
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
	bIsUnitRelocationMode = true;
	UnitWaitingRelocationFromUI = Unit;
}

void ASTSPlayerController::OnButtonStartPortal()
{
	LockedShip = nullptr;
	SetToPortalSelectionMode();
}

void ASTSPlayerController::OnButtonCancelPortal()
{
	SetToViewMode();
}

void ASTSPlayerController::OnButtonRemoveBuilding()
{
	UE_LOG(LogTemp, Warning, TEXT("Remove Building"));
	Commander->DestroyBuildingFromGame(Commander->GetTargetResoureTile()->GetBuilding());
}

void ASTSPlayerController::OnButtonRemoveShip()
{
	if (RemovingShipTarget)
	{
		RemovingShipTarget->RemoveAllUnitsFromGame();
		Commander->DestroyShipFromGame(RemovingShipTarget);
	}
}

void ASTSPlayerController::OnButtonRemoveUnit()
{
	if (RemovingUnitTarget)
	{
		Commander->DestroyUnitFromGame(RemovingUnitTarget);
	}
}

void ASTSPlayerController::OnButtonLookIslandUnderAttack(int32 Index)
{
	if (IslandsUnderAttack.IsValidIndex(Index))
	{
		PlayerCommander->MoveCommanderToTile(IslandsUnderAttack[Index], true);
	}
}

void ASTSPlayerController::OnButtonLookShipUnderAttacking(int32 Index)
{
	if (ShipsAttacking.IsValidIndex(Index) && ShipsAttacking[Index]->GetCurTile())
	{
		PlayerCommander->MoveCommanderToTile(ShipsAttacking[Index]->GetCurTile(), true);
	}
}

void ASTSPlayerController::OnButtonLookOwningShip(AShip* Ship)
{
	if (!Ship)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullptr ship to look."));
		return;
	}
	LockedShip = Ship;
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
	EnhancedInputComponent->BindAction(InputMouseReleasedForPortal, ETriggerEvent::Triggered, this, &ASTSPlayerController::MouseReleasedForPortal);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = ViewTileMode;
		Subsystem->AddMappingContext(NewMappingContext, 0);
	}
}

void ASTSPlayerController::MoveCamera(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector NewLocation = Commander->GetActorLocation() + FVector(MovementVector.X, MovementVector.Y, 0.0f) * CameraMovementSpeed;
	NewLocation.X = FMath::Clamp(NewLocation.X, 475, 11350);
	NewLocation.Y = FMath::Clamp(NewLocation.Y, 1125, 18375);
	Commander->SetActorLocation(NewLocation);
}

void ASTSPlayerController::ZoomCamera(const FInputActionValue& Value)
{
	float Zoom = Value.Get<float>();
	PlayerCommander->SpringArmComp->TargetArmLength = FMath::Clamp(PlayerCommander->SpringArmComp->TargetArmLength + CameraZoomSpeed * Zoom, 2000, 14000);


	if (PlayerCommander->SpringArmComp->TargetArmLength > 8000 && PrevArmLength <= 8000)
	{
		GetGameInstance()->GetSubsystem<UMapManager>()->SetResoureUIVisibility(false);
	}
	else if (PlayerCommander->SpringArmComp->TargetArmLength <= 8000 && PrevArmLength > 8000)
	{
		GetGameInstance()->GetSubsystem<UMapManager>()->SetResoureUIVisibility(true);
	}
	PrevArmLength = PlayerCommander->SpringArmComp->TargetArmLength;
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
		AIslandTile* SecondTile = Cast<AIslandTile>(Tile);

		if (!SecondTile || SecondTile == FirstTile) Tile->OnTileSelectedAsView(this);
		LockedShip = nullptr;

		if (bIsUnitRelocationMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("From UI"));
			Commander->TryRelocateUnitOnTile(UnitWaitingRelocationFromUI, SecondTile);
			bIsUnitRelocationMode = false;
		}
		else
		{
			if (FirstTile && SecondTile && FirstTile != SecondTile)
			{
				Commander->TryRelocateUnitsOfTwoTiles(FirstTile, SecondTile);
			}
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

void ASTSPlayerController::MouseReleasedForPortal(const FInputActionValue& Value)
{
	ABaseTile* Tile = MouseRay();
	if (Tile && Commander->GetTargetResoureTile() && Commander->GetTargetResoureTile()->GetBuilding() && Commander->GetTargetResoureTile()->GetBuilding()->GetBuildingType() == EBuildingType::Portal)
	{
		AResourceTile* ResourceTile = Cast<AResourceTile>(Tile);
		if (ResourceTile && ResourceTile->GetBuilding() && ResourceTile->GetBuilding()->GetBuildingType() == EBuildingType::Portal)
		{
			if (Cast<APortal>(Commander->GetTargetResoureTile()->GetBuilding())->SendShipToTile(Tile))
			{
				SetToViewMode();
			}
		}
	}
	else SetToViewMode();
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
		if (UserInputMode == EUserInputMode::View) Commander->SetActorLocation(LockedShip->GetActorLocation());
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

	IslandsUnderAttack.Empty();
	for (int32 Index = Commander->OwningIslands.Num() - 1; Index >= 0; Index--)
	{
		if (!Commander->OwningIslands.IsValidIndex(Index)) continue;
		AGuardianTile* GuardianTile = Commander->OwningIslands[Index];
		if (!GuardianTile) continue;
		if (GuardianTile->bIsAttackedRecently) IslandsUnderAttack.Add(GuardianTile);
	}
	ShipsAttacking.Empty();
	for (int32 Index = Commander->Ships.Num() - 1; Index >= 0; Index--)
	{
		if (!Commander->Ships.IsValidIndex(Index)) continue;
		AShip* Ship = Commander->Ships[Index];
		if (!Ship) continue;
		if (Ship->bIsAttackedRecently) ShipsAttacking.Add(Ship);
	}
}
