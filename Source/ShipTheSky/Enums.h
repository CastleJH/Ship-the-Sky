// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Island = 0,
	Cloud,
	Storm,
	Sun,
	Lightning,
	Meteor,
	End
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	StoneCloud = 0 UMETA(DisplayName = "StoneCloud"),
	StoneStorm UMETA(DisplayName = "StoneStorm"),
	StoneSun UMETA(DisplayName = "StoneSun"),
	StoneLightning UMETA(DisplayName = "StoneLightning"),
	StoneMeteor UMETA(DisplayName = "StoneMeteor"),
	WoodCloud UMETA(DisplayName = "WoodCloud"),
	WoodStorm UMETA(DisplayName = "WoodStorm"),
	WoodSun UMETA(DisplayName = "WoodSun"),
	WoodLightning UMETA(DisplayName = "WoodLightning"),
	WoodMeteor UMETA(DisplayName = "WoodMeteor"),
	Food UMETA(DisplayName = "Food"),
	End UMETA(DisplayName = "End")
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Miner = 0,
	Woodcutter,
	Farmer,
	Warrior,
	None
};

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Barracks = 0,
	Shipyard,
	Portal,
	Sanctuary
};