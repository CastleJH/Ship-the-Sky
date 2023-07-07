// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Portal.h"
#include "Tile/BaseTile.h"
#include "Tile/ResourceTile.h"
#include "Enums.h"
#include "Ship.h"
#include "Controller/STSPlayerController.h"

APortal::APortal()
{
	BuildingType = EBuildingType::Portal;
	PortalCurCooltime = 0;
	PortalMaxCooltime = 10;
}

bool APortal::SendShipToTile(ABaseTile* Tile)
{
	UE_LOG(LogTemp, Warning, TEXT("Portal1"));
	if (PortalCurCooltime != 0) return false;
	if (!CurTile->GetShip()) return false;

	if (Tile && Tile->GetTileType() == ETileType::Island && !Tile->GetShip())
	{
		AResourceTile* ResourceTile = Cast<AResourceTile>(Tile);
		if (ResourceTile && ResourceTile->GetBuilding() && ResourceTile->GetBuilding()->GetBuildingType() == EBuildingType::Portal)
		{
			CurTile->GetShip()->Teleport(Tile);
			PortalCurCooltime = PortalMaxCooltime;
			Cast<ASTSPlayerController>(GetWorld()->GetFirstPlayerController())->SetToViewMode();
			UE_LOG(LogTemp, Warning, TEXT("Portal2"));
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("Portal3"));
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Portal4"));
	return false;
}

void APortal::IncreaseProgress()
{
	PortalCurCooltime = FMath::Clamp(PortalCurCooltime - 1, 0, PortalMaxCooltime);
}
