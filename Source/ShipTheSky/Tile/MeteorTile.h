// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/BaseTile.h"
#include "MeteorTile.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API AMeteorTile : public ABaseTile
{
	GENERATED_BODY()

public:
	AMeteorTile();

	virtual void TimePass(int32 GameDate) override;
};
