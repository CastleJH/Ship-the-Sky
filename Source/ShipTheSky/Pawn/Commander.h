// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enums.h"
#include "Commander.generated.h"


UCLASS()
class SHIPTHESKY_API ACommander : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommander();

public:
	//�Ǽ��� �ǹ� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> BarracksClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> ShipyardClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> PortalClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> SanctuaryClass;
	
	//���༱ Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class AShip> ShipClass;

	//���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> MinerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> WoodcutterClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> FarmerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> WarriorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	int32 OutlineColorIndex;

protected:
	//���ְ��� �۾� ���� Ÿ�ٵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander", meta = (AllowPrivateAccess = "true"))
	class ABaseTile* TargetTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander", meta = (AllowPrivateAccess = "true"))
	class AIslandTile* TargetIslandTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander", meta = (AllowPrivateAccess = "true"))
	class AResourceTile* TargetResourceTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander", meta = (AllowPrivateAccess = "true"))
	class AGuardianTile* TargetGuardianTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander", meta = (AllowPrivateAccess = "true"))
	class AShip* TargetShip;

	//���ְ��� ������ �ڿ�
	UPROPERTY(VisibleAnywhere)
	TArray<int32> Resources;

public:
	//���ְ��� ���� �� �ִ� ���
	UFUNCTION(BlueprintCallable)
	void ConstructBuilding(class AResourceTile* Tile, enum EBuildingType Type);
	UFUNCTION(BlueprintCallable)
	bool TryCreateUnit(class ABarracks* Barracks, enum EUnitType Type);
	UFUNCTION(BlueprintCallable)
	bool TryCreateShip(class AShipyard* Shipyard);
	UFUNCTION(BlueprintCallable)
	bool TryEmbarkUnit(class AShip* Ship, class ABaseUnit* Unit);
	UFUNCTION(BlueprintCallable)
	bool TryDisembarkUnit(class ABaseUnit* Unit);
	UFUNCTION(BlueprintCallable)
	bool TryDepartShip(class AShip* Ship);
	UFUNCTION(BlueprintCallable)
	bool TryStopShip(class AShip* Ship);
	UFUNCTION(BlueprintCallable)
	void FillIslandWithUnit(int32 IslandID, class ABaseUnit* Unit);

	//���ְ� ���� ������ ����
	UFUNCTION(BlueprintCallable)
	void SetTargetTile(class ABaseTile* NewTile);
	UFUNCTION(BlueprintPure)
	class ABaseTile* GetTargetTile() const { return TargetTile; }
	UFUNCTION(BlueprintPure)
	class AIslandTile* GetTargetIslandTile() const { return TargetIslandTile; }
	UFUNCTION(BlueprintPure)
	class AResourceTile* GetTargetResoureTile() const { return TargetResourceTile; }
	UFUNCTION(BlueprintPure)
	class AGuardianTile* GetTargetGuardianTile() const { return TargetGuardianTile; }
	UFUNCTION(BlueprintCallable)
	void SetTargetShip(class AShip* Ship) { TargetShip = Ship; }
	UFUNCTION(BlueprintPure)
	class AShip* GetTargetShip() const { return TargetShip; }
	void SetResource(int32 Amount, EResourceType Type) { Resources[(int32)Type] = Amount; }
	UFUNCTION(BlueprintPure)
	int32 GetResource(EResourceType Type) const { return Resources[(int32)Type]; }
};
