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
	Cloud = 1,
	Storm = 2,
	Sun = 3,
	Lightning = 4,
	Meteor = 5,
	None
};

/*
Stone = TileType - 1;
Wood = TileType + 4;
*/

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	StoneCloud = 0,
	StoneStorm = 1,
	StoneSun = 2,
	StoneLightning = 3,
	StoneMeteor = 4,
	WoodCloud = 5,
	WoodStorm = 6,
	WoodSun = 7,
	WoodLightning = 8,
	WoodMeteor = 9,
	Food = 10,
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

UENUM(BlueprintType)
enum class EUnitStat : uint8
{
	HP = 0,
	Attack,
	FoodConsume,
	Efficiency,
	All,
	None
};

UENUM(BlueprintType)
enum class EGuardianStat : uint8
{
	HPCloud = 0,
	AttackStorm,
	HPSun,
	AttackLightning,
	Score,
	None
};

UENUM(BlueprintType)
enum class EShipStatus : uint8
{
	MoveForUpgrade = 0,
	MoveForEmbarkUnit,
	MoveForBattle,
	InBattle = 0,
	Stuck = 0,
	None
};