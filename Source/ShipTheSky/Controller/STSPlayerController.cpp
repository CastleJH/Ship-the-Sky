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

ASTSPlayerController::ASTSPlayerController()
{
	CameraMovementSpeed = 4000.0f;

	bIsPathSelectionMode = false;
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
	OpenOwningIslandBuildingUI();
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
	CloseOwningIslandUI();
	Commander->TryDepartShip(Commander->GetTargetShip());
}

void ASTSPlayerController::SetIsPathSelectionMode(bool IsPathSelectionMode)
{
	bIsPathSelectionMode = IsPathSelectionMode;
	if (bIsPathSelectionMode)
	{
		CloseOwningIslandUI();
		CloseShipUI();
		CloseShipUI();
		
		PathSelectionUI->AddToViewport();
	}
	else
	{
		OpenOwningIslandUI();
		OpenShipUI();

		PathSelectionUI->RemoveFromViewport();
	}
}

void ASTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveRight"), this, &ASTSPlayerController::MoveCameraHorizontal);
	InputComponent->BindAxis(TEXT("MoveForward"), this, &ASTSPlayerController::MoveCameraVertical);
	InputComponent->BindAxis(TEXT("Zoom"), this, &ASTSPlayerController::ZoomCamera);
	//InputComponent->BindAction(TEXT("MouseRay"), EInputEvent::IE_Repeat, this, &ASTSPlayerController::MouseRay);
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
	PathSelectionUI = CreateWidget<UUserWidget>(GetWorld(), PathSelectionUIClass);
	if (PathSelectionUI != nullptr)
	{
		//PathSelectionUI->AddToViewport();
		//ClosePathSelectionUI();
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

void ASTSPlayerController::MouseRay()
{
	FHitResult OutResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, OutResult);
	if (OutResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %s"), *OutResult.GetActor()->GetActorNameOrLabel(), *OutResult.GetActor()->GetActorLocation().ToString());
		if (GetCommander()->GetTargetShip() == nullptr) return;
		GetCommander()->GetTargetShip()->TryAddTileToPath(Cast<ABaseTile>(OutResult.GetActor()));
	}
}
