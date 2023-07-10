// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/BaseTile.h"
#include "SunTile.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API ASunTile : public ABaseTile
{
	GENERATED_BODY()

public:
	ASunTile();

	virtual void TimePass(int32 GameDate) override;
};
