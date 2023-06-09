// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Commander.generated.h"

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
	End
};

UCLASS()
class SHIPTHESKY_API ACommander : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommander();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> BarracksClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> ShipyardClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> PortalClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class ABaseBuilding> SanctuaryClass;
	UPROPERTY(EditDefaultsOnly, Category = "Commander")
	TSubclassOf<class AShip> ShipClass;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander")
	class ABaseTile* TargetTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commander")
	class AShip* TargetShip;

	TArray<int32> Resources;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ConstructBuilding(class AResourceTile* Tile, enum EBuildingType Type);

	void SetTargetTile(class ABaseTile* NewTile);

	class ABaseTile* GetTargetTile() const { return TargetTile; }
	UFUNCTION(BlueprintPure)
	class AIslandTile* GetTargetIslandTile() const { return TargetIslandTile; }

	void SetTargetShip(class AShip* Ship) { TargetShip = Ship; }

	class AShip* GetTargetShip() const { return TargetShip; }

	void SetResource(int32 Amount, EResourceType Type) { Resources[(int32)Type] = Amount; }

	UFUNCTION(BlueprintPure)
	int32 GetResource(EResourceType Type) const { return Resources[(int32)Type]; }

	void FillIslandWithUnit(int32 IslandID, class ABaseUnit* Unit);
};
