// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/BaseBuilding.h"
#include "Portal.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API APortal : public ABaseBuilding
{
	GENERATED_BODY()
	
public:
	APortal();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PortalMaxCooltime;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PortalCurCooltime;

public:
	UFUNCTION(BlueprintCallable)
	bool SendShipToTile(class ABaseTile* Tile);

	//진행 상황 업데이트
	virtual void IncreaseProgress() override;

};
