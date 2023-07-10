// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/BaseTile.h"
#include "LightningTile.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API ALightningTile : public ABaseTile
{
	GENERATED_BODY()
	
public:
	ALightningTile();

	virtual void TimePass(int32 GameDate) override;
};
