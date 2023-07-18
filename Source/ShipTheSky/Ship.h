// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
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
	//경로
	UPROPERTY(VisibleAnywhere)
	TArray<class ABaseTile*> Path;

	//탑승 유닛
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class ABaseUnit*> Units;
	
	//현재 위치
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class ABaseTile* CurTile;

	//비행선 식별
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString ShipName;

	//비행선 스탯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxDurability;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurDurability;
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
	float OriginalFlightPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ModifiedFlightPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 UnitCapacity;

	//업그레이드 관련
	int32 DurabilityStormLevel;
	int32 DurabilityLightningLevel;
	int32 FlightPowerCloudLevel;
	int32 FlightPowerSunLevel;
	int32 CloudLevel;
	int32 StormLevel;
	int32 SunLevel;
	int32 LightningLevel;
	int32 MeteorLevel;
	int32 ResistanceDelta;
	int32 DurabilityDelta;
	float FlightPowerDelta;

	FTimerHandle MoveTimer;
	UPROPERTY(VisibleAnywhere)
	class ACommander* OwnerCommander;

	//적 AI만 다룸
	EShipStatus ShipStatus;
	int32 StuckCount;

public:

	void Tick(float DeltaSeconds) override;

	bool TryLocateOnTile(class ABaseTile* Tile, bool RightAfter);

	//비행선 관련 변수 접근
	UFUNCTION(BlueprintPure)
	FString GetShipName() const { return ShipName; }
	UFUNCTION(BlueprintPure)
	void SetShipName(FString& NewName) { ShipName = NewName; }

	UFUNCTION(BlueprintPure)
	class ABaseTile* GetCurTile() const { return CurTile; }

	UFUNCTION(BlueprintPure)
	float GetMaxDurability() const { return MaxDurability; }
	void SetMaxDurability(float NewDurability);
	UFUNCTION(BlueprintPure)
	float GetCurDurability() const { return CurDurability; }
	void SetCurDurability(float NewDurability);

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
	float GetOriginalFlightPower() const { return OriginalFlightPower; }
	UFUNCTION(BlueprintPure)
	float GetModifiedFlightPower() const { return ModifiedFlightPower; }
	void SetOriginalFlightPower(float NewFlightPower) { OriginalFlightPower = NewFlightPower; }
	void SetModifiedFlightPower(float NewFlightPower) { ModifiedFlightPower = NewFlightPower; }

	UFUNCTION(BlueprintPure)
	int32 GetUnitCapacity() const { return UnitCapacity; }
	void SetUnitCapacity(int32 NewUnitCapacity) { UnitCapacity = NewUnitCapacity; }

	UPROPERTY()
	TArray<class AGuardian*> TargetGuardians;

	UFUNCTION()
	void SetOwnerCommander(class ACommander* NewCommnader) { OwnerCommander = NewCommnader; }
	UFUNCTION(BlueprintPure)
	class ACommander* GetOwnerCommander() const { return OwnerCommander; }

	//비행 관련
	bool AddUnit(class ABaseUnit* Unit);
	bool RemoveUnit(class ABaseUnit* Unit);

	bool TryAddTileToPath(class ABaseTile* Tile, bool bIsFirstPath);
	UFUNCTION(BlueprintCallable)
	void EmptyPath();
	void FollowPath();
	void Teleport(class ABaseTile* Tile);

	//경로 UI/비행선 UI 관련
	void ClearPathUI();
	void UpdatePathUI();
	void RemoveFrontPathUI();
	UPROPERTY()
	bool bIsBeingObserved;
	UFUNCTION(BlueprintCallable)
	FString GetStatusString();

	//전투 관련
	void GetAttacked(float Damage);
	float Attack();
	bool CanShipAttack() { return Units.Num() != 0; }
	float LastAttackedSecond = 0.0f;
	bool bIsAttackedRecently = false;

	//제거 관련
	void RemoveAllUnitsFromGame();

	//타일 관련
	void ConsumeFoodForUnits(float Multiply);
	void DecreaseHPForUnits(float Amount);

	//업그레이드 관련
	UFUNCTION(BlueprintCallable)
	bool UpgradeResistance(enum ETileType Type);
	UFUNCTION(BlueprintPure)
	int32 GetResistanceUpgradeCost(enum ETileType Type) const;
	UFUNCTION(BlueprintCallable)
	bool UpgradeFlightPowerWithCloud();
	UFUNCTION(BlueprintPure)
	int32 GetFlightPowerUpgradeCostWithCloud() const { return FlightPowerCloudLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeFlightPowerWithSun();
	UFUNCTION(BlueprintPure)
	int32 GetFlightPowerUpgradeCostWithSun() const { return FlightPowerSunLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeDurabilityWithStorm();
	UFUNCTION(BlueprintPure)
	int32 GetDurabilityUpgradeCostWithStorm() const { return DurabilityStormLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeDurabilityWithLightning();
	UFUNCTION(BlueprintPure)
	int32 GetDurabilityUpgradeCostWithLightning() const { return DurabilityLightningLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeCapacity();
	UFUNCTION(BlueprintPure)
	int32 GetCapacityUpgradeCost() const;

	void RecoverDurability(float Amount);

	//적 AI만
	EShipStatus GetShipStatus() const { return ShipStatus; }
	void SetShipStatus(EShipStatus NewStatus) { ShipStatus = NewStatus; }
};
