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
	//건설할 건물 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> BarracksClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> ShipyardClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> PortalClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> SanctuaryClass;
	
	//비행선 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class AShip> ShipClass;

	//유닛 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> MinerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> WoodcutterClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> FarmerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> WarriorClass;

	//상징 색
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	int32 OutlineColorIndex;

	//시작 섬 위치
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	int32 StartRow;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	int32 StartCol;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	int32 EmptyStartIndex;

	UPROPERTY(VisibleAnywhere)
	TArray<class AGuardianTile*> OwningIslands;
	

protected:
	//지휘관이 작업 중인 타겟들
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

	//지휘관 자원 관련
	UPROPERTY(VisibleAnywhere)
	TArray<float> Resources;
	UPROPERTY(VisibleAnywhere)
	int32 UnitCreationCost;
	UPROPERTY(VisibleAnywhere)
	int32 ShipCreationCost;

public:
	//지휘관 소유 유닛/건물/비행선
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class ABaseUnit*> Units;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class ABaseBuilding*> Buildings;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class AShip*> Ships;

public:
	//지휘관이 내릴 수 있는 명령
	UFUNCTION(BlueprintCallable)
	bool TryConstructBuilding(class AResourceTile* Tile, enum EBuildingType Type);
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
	void TryRelocateUnitsOfTwoTiles(AIslandTile* Tile1, AIslandTile* Tile2);
	void TryRelocateUnitOnTile(ABaseUnit* Unit, AIslandTile* Tile);
	UFUNCTION(BlueprintCallable)
	void FillIslandWithUnit(int32 IslandID, class ABaseUnit* Unit);

	//지휘관 관련 변수들 접근
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

	//유닛/건물/비행선 생성/제거
	UFUNCTION()
	class ABaseUnit* SpawnUnitToGame(enum EUnitType Type);
	UFUNCTION()
	void DestroyUnitFromGame(class ABaseUnit* Unit);
	UFUNCTION()
	class ABaseBuilding* SpawnBuildingToGame(enum EBuildingType Type, FVector Location, FRotator Rotation);
	UFUNCTION()
	void DestroyBuildingFromGame(class ABaseBuilding* Building);
	UFUNCTION()
	class AShip* SpawnShipToGame();
	UFUNCTION()
	void DestroyShipFromGame(class AShip* Ship);

	//자원 관련 변수
	UFUNCTION(BlueprintCallable)
	float GetUnitCreationCost() { return UnitCreationCost; }
	UFUNCTION(BlueprintCallable)
	void DecreaseUnitCreationCost() { UnitCreationCost -= 10; }
	UFUNCTION(BlueprintCallable)
	void IncreaseUnitCreationCost() { UnitCreationCost += 10; }
	UFUNCTION(BlueprintCallable)
	float GetShipCreationCost() { return ShipCreationCost; }
	UFUNCTION(BlueprintCallable)
	void DecreaseShipCreationCost() { ShipCreationCost -= 50; }
	UFUNCTION(BlueprintCallable)
	void IncreaseShipCreationCost() { ShipCreationCost += 50; }
	UFUNCTION(BlueprintCallable)
	float GetBuildingCreationCost(enum EBuildingType Type);
	UFUNCTION(BlueprintPure)
	class AShip* GetTargetShip() const { return TargetShip; }
	void SetResource(float Amount, EResourceType Type);
	UFUNCTION(BlueprintPure)
	float GetResource(EResourceType Type) const { return Resources[(int32)Type]; }
};
