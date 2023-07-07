// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EUserInputMode : uint8
{
	View = 0,
	PathSelection,
	PortalSelecton
};

UENUM(BlueprintType)
enum class EIslandTileType : uint8
{
	Mine = 0,
	Forest,
	Farm,
	Guardian
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Island = 0,
	Cloud,
	Storm,
	Sun,
	Lightning,
	Meteor,
	None
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	StoneCloud = 0,
	StoneStorm,
	StoneSun,
	StoneLightning,
	StoneMeteor,
	WoodCloud,
	WoodStorm,
	WoodSun,
	WoodLightning,
	WoodMeteor,
	Food,
	None
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
	Sanctuary,
	None
};