// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/BaseTile.h"
#include "Controller/STSPlayerController.h"
#include "Pawn/Commander.h"
#include "Ship.h"
#include "Components/WidgetComponent.h"
#include "BaseTile.h"
#include "IslandTile.h"
#include "STSGameState.h"

// Sets default values
ABaseTile::ABaseTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->CastShadow = false;

	TileWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Tile UI"));

	TileWidgetComp->SetupAttachment(RootComponent);
	TileWidgetComp->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	TileWidgetComp->SetDrawSize(FVector2D(75.0f, 40.0f));
	TileWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	TileWidgetComp->SetTickMode(ETickMode::Disabled);
	TileWidgetComp->SetWidgetClass(LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/TileUI/WBP_TileUI.WBP_TileUI_C")));
}

void ABaseTile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Cast<ACommander>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetTargetShip(ShipOnThisTile);
}

void ABaseTile::TimePass(int32 GameDate)
{
}

void ABaseTile::SetTileUI(int32 Number)
{
	if (Number == 0)
	{
		TileWidgetComp->GetWidget()->SetVisibility(ESlateVisibility::Collapsed);
		TileWidgetComp->SetTickMode(ETickMode::Disabled);
	}
	else
	{
		TileWidgetComp->SetTickMode(ETickMode::Automatic);
		OnSetTileUI(Number);
		TileWidgetComp->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABaseTile::SetOutline()
{
	ACommander* PlayerCommander = Cast<ACommander>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCommander->GetTargetTile() == this)
	{
		StaticMeshComp->SetRenderCustomDepth(false);
		StaticMeshComp->CustomDepthStencilValue = 8;
		StaticMeshComp->SetRenderCustomDepth(true);
	}
	else if (TileType == ETileType::Island)
	{
		 ACommander* IslandOwner = GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(Cast<AIslandTile>(this)->GetIslandID());
		 if (IslandOwner)
		 {
			 StaticMeshComp->SetRenderCustomDepth(false);
			 StaticMeshComp->CustomDepthStencilValue = IslandOwner->OutlineColorIndex;
			 StaticMeshComp->SetRenderCustomDepth(true);
		 }
		 else StaticMeshComp->SetRenderCustomDepth(false);
	}
	else StaticMeshComp->SetRenderCustomDepth(false);
}

// Called when the game starts or when spawned
void ABaseTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	SetActorScale3D(FVector(0.94f, 0.975f, 0.975f));
}

void ABaseTile::OnTileSelectedAsView(ASTSPlayerController* PlayerController)
{
	if (PlayerController->GetUserInputMode() == EUserInputMode::View)
	{

		ABaseTile* CurrentTargetTile = PlayerController->GetCommander()->GetTargetTile();
		if (CurrentTargetTile != nullptr)
		{
			AIslandTile* IslandTile = Cast<AIslandTile>(CurrentTargetTile);
			ACommander* IslandOwner = nullptr;
			if (IslandTile) IslandOwner = GetWorld()->GetGameState<ASTSGameState>()->GetIslandOwner(IslandTile->GetIslandID());
			CurrentTargetTile->SetActorTickEnabled(false);
			if (CurrentTargetTile == this) PlayerController->GetCommander()->SetTargetTile(nullptr);
			else
			{
				PlayerController->GetCommander()->SetTargetTile(this);
				SetActorTickEnabled(true);
			}
			CurrentTargetTile->SetOutline();
		}
		else
		{
			PlayerController->GetCommander()->SetTargetTile(this);
			SetActorTickEnabled(true);
		}
	}
	SetOutline();
}

bool ABaseTile::OnTileFirstSelectedAsPath(ASTSPlayerController* PlayerController)
{
	if (PlayerController->GetUserInputMode() == EUserInputMode::PathSelection)
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
	if (PlayerController->GetUserInputMode() == EUserInputMode::PathSelection)
	{
		if (PlayerController->GetCommander()->GetTargetShip() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Ship"));
			return;
		}
		PlayerController->GetCommander()->GetTargetShip()->TryAddTileToPath(this, false);
	}
}
