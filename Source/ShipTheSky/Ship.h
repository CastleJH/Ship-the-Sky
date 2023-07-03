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
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	
private:
	//탑승 유닛
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class ABaseUnit*> Units;
	
	//현재 위치
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class ABaseTile* CurTile;

	//비행선 스탯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	int32 Durability;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CloudResistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 StormResistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SunResistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 LightningResistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MeteorResistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 FlightPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 UnitCapacity;

	//비행 관련 변수
	UPROPERTY(VisibleAnywhere)
	TArray<class ABaseTile*> Path;
	FTimerHandle MoveTimer;

public:
	void Tick(float DeltaTime) override;

	bool TryLocateOnTile(class ABaseTile* Tile, bool RightAfter);

	void InitializeStatWithResources(int32 WoodCloud, int32 WoodStorm, int32 WoodSun, int32 WoodLightning, int32 WoodMeteor);

	//비행선 관련 변수 접근
	class ABaseTile* GetCurTile() const { return CurTile; }

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

	//비행 관련
	bool AddUnit(class ABaseUnit* Unit);
	bool RemoveUnit(class ABaseUnit* Unit);

	bool TryAddTileToPath(class ABaseTile* Tile, bool bIsFirstPath);
	UFUNCTION(BlueprintCallable)
	void EmptyPath();

	void FollowPath();

};
