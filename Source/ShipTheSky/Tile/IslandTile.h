// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/BaseTile.h"
#include "IslandTile.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EIslandTileType : uint8
{
	Mine = 0,
	Forest,
	Farm,
	Guardian
};

UCLASS()
class SHIPTHESKY_API AIslandTile : public ABaseTile
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AIslandTile();

	//고쳐요 나중에
	UPROPERTY(VisibleAnywhere)
	TMap<uint8, int32> Resources;

protected:
	//UPROPERTY(VisibleAnywhere, Category = UI)
	//class UWidgetComponent* WidgetComponent;
	//UPROPERTY(EditDefaultsOnly, Category = "Widget")
	//TSubclassOf<class UUserWidget> IslandResourceUIClass;

private:
	//class UIslandResourceUI* IslandResourceUI;

	UPROPERTY(VisibleAnywhere)
	int32 IslandID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EIslandTileType IslandType;

	UPROPERTY()
	class ABaseBuilding* BuildingOnThisTile = nullptr;
	UPROPERTY()
	class ABaseUnit* UnitOnThisTile = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetIslandID() const { return IslandID; }
	void SetIslandID(int32 NewIslandID) { IslandID = NewIslandID; }

	void SetBuilding(class ABaseBuilding* NewBuilding) { BuildingOnThisTile = NewBuilding; }
	UFUNCTION(BlueprintPure)
	class ABaseBuilding* GetBuilding() const { return BuildingOnThisTile; }

	void SetUnit(class ABaseUnit* NewUnit) { UnitOnThisTile = NewUnit; }
	UFUNCTION(BlueprintPure)
	class ABaseUnit* GetUnit() const { return UnitOnThisTile; }

	EIslandTileType GetIslandType() const { return IslandType; };
	virtual void TimePass(int32 GameDate) override;

	void SetResources(float Power);

protected:
	virtual void BeginPlay() override;
	virtual void OnTileReleased(AActor* Target, FKey ButtonPressed) override;

	void GiveResourceToUnit();
	void GiveProgressToBuilding();
};
