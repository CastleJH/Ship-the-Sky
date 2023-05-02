// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Commander.generated.h"

UCLASS()
class SHIPTHESKY_API ACommander : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommander();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> BarracksClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> ShipyardClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> PortalClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> SanctuaryClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> MinerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> WoodcutterClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> FarmerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseUnit> WarriorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander")
	class AIslandTile* TargetIslandTile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FillIslandWithUnit(int32 IslandID, class ABaseUnit* Unit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void CreateUnit(int32 IslandID, enum EUnitType Type);

	UFUNCTION(BlueprintCallable)
	void ConstructBuilding(class AIslandTile* Tile, enum EBuildingType Type);
	
	void SetTargetIslandTile(class AIslandTile* NewTile) { TargetIslandTile = NewTile; }
	class AIslandTile* GetTargetIslandTile() const { return TargetIslandTile; }
};
