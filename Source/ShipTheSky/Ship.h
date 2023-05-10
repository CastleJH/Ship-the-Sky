// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship.generated.h"

UCLASS()
class SHIPTHESKY_API AShip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShip();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Ship Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComp;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class ABaseTile* CurTile;

	UPROPERTY()
	int32 Durability;
	UPROPERTY()
	int32 CloudResistance;
	UPROPERTY()
	int32 StormResistance;
	UPROPERTY()
	int32 SunResistance;
	UPROPERTY()
	int32 LightningResistance;
	UPROPERTY()
	int32 MeteorResistance;
	UPROPERTY()
	int32 FlightPower;
	UPROPERTY()
	int32 UnitCapacity;

public:
	void LocateOnTile(class ABaseTile* Tile);

	void InitializeStatWithResources(int32 WoodCloud, int32 WoodStorm, int32 WoodSun, int32 WoodLightning, int32 WoodMeteor);

	class ABaseTile* GetCurTile() const { return CurTile; }
	void SetCurTile(class ABaseTile* NewTile) { CurTile = NewTile; }

	UFUNCTION(BlueprintPure)
	int32 GetDurability() const { return Durability; }
	void SetDurability(int32 NewDurability) { Durability = NewDurability; }

	UFUNCTION(BlueprintPure)
	int32 GetCloudResistance() const { return CloudResistance; }
	void SetCloudResistance(int32 NewCloudResistance) { CloudResistance = NewCloudResistance; }

	UFUNCTION(BlueprintPure)
	int32 GetStormResistance() const { return StormResistance; }
	void SetStormResistance(int32 NewStormResistance) { StormResistance = NewStormResistance; }

	UFUNCTION(BlueprintPure)
	int32 GetSunResistance() const { return SunResistance; }
	void SetSunResistance(int32 NewSunResistance) { SunResistance = NewSunResistance; }

	UFUNCTION(BlueprintPure)
	int32 GetLightningResistance() const { return LightningResistance; }
	void SetLightningResistance(int32 NewLightningResistance) { LightningResistance = NewLightningResistance; }

	UFUNCTION(BlueprintPure)
	int32 GetMeteorResistance() const { return MeteorResistance; }
	void SetMeteorResistance(int32 NewMeteorResistance) { MeteorResistance = NewMeteorResistance; }

	UFUNCTION(BlueprintPure)
	int32 GetFlightPower() const { return FlightPower; }
	void SetFlightPower(int32 NewFlightPower) { FlightPower = NewFlightPower; }

	UFUNCTION(BlueprintPure)
	int32 GetUnitCapacity() const { return UnitCapacity; }
	void SetUnitCapacity(int32 NewUnitCapacity) { UnitCapacity = NewUnitCapacity; }
};
