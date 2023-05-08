// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShipTheSkyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API AShipTheSkyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay();
	
};
