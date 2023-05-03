// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/STSPlayerController.h"
#include "STSGameState.h"
#include "Pawn/Commander.h"
#include "Pawn/PlayerCommander.h"
#include "Tile/IslandTile.h"
#include "Building/BaseBuilding.h"
#include "Widget/IslandTileUI.h"
#include "Widget/ResourceUI.h"
#include "MapManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ASTSPlayerController::ASTSPlayerController()
{
	CameraMovementSpeed = 4000.0f;
}

void ASTSPlayerController::SetIslandTileUIVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		SetIslandTileUIVisibility(false);
		if (Cast<ASTSGameState>(GetWorld()->GetGameState())->GetIslandOwner(Commander->GetTargetIslandTile()->GetIslandID()) == this)
		{
			IslandTileUI->SetPanelVisibility(IslandTileUI->ConstructionPanel, ESlateVisibility::Visible);
			if (Commander->GetTargetIslandTile()->GetBuilding() != nullptr)
			{
				switch (Commander->GetTargetIslandTile()->GetBuilding()->GetBuildingType())
				{
				case EBuildingType::Barracks:
					IslandTileUI->SetPanelVisibility(IslandTileUI->BarracksPanel, ESlateVisibility::Visible);
					break;
				}
			}
		}
	}
	else
	{
		IslandTileUI->SetPanelVisibility(IslandTileUI->ConstructionPanel, ESlateVisibility::Hidden);
		IslandTileUI->SetPanelVisibility(IslandTileUI->BarracksPanel, ESlateVisibility::Hidden);
	}
}

void ASTSPlayerController::OnButtonCreateUnitPressed(EUnitType Type)
{
	Commander->CreateUnit(Commander->GetTargetIslandTile()->GetIslandID(), Type);
}

void ASTSPlayerController::OnButtonConstructBuildingPressed(EBuildingType Type)
{
	Commander->ConstructBuilding(Commander->GetTargetIslandTile(), Type);
	IslandTileUI->SetPanelVisibility(IslandTileUI->BarracksPanel, ESlateVisibility::Visible);
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
		SetIslandTileUIVisibility(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UI"));
	}
	ResourceUI = Cast<UResourceUI>(CreateWidget<UUserWidget>(GetWorld(), ResourceUIClass));
	if (ResourceUI != nullptr)
	{
		ResourceUI->AddToViewport();
		ResourceUI->PlayerCommander = Cast<APlayerCommander>(Commander);
		Cast<APlayerCommander>(Commander)->OnResourceChanged.BindUObject(ResourceUI, &UResourceUI::SetResourceText);
		for (int32 Resource = 0; Resource < (int32)EResourceType::End; Resource++)
		{
			ResourceUI->SetResourceText(StaticCast<EResourceType>(Resource));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UI"));
	}
	GetGameInstance()->GetSubsystem<UMapManager>()->GenerateMap(75);
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
