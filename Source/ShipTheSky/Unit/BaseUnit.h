// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "BaseUnit.generated.h"


UCLASS()
class SHIPTHESKY_API ABaseUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseUnit();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Unit Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	EUnitType UnitType;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class AIslandTile* CurIslandTile;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class AShip* CurShip;

public:
	void LocateToResourceTile(class AResourceTile* ResourceTile);
	bool Embark(class AShip* Ship);
	bool Disembark();

	UFUNCTION(BlueprintPure)
	EUnitType GetUnitType() const { return UnitType; }
	UFUNCTION(BlueprintPure)
	class AIslandTile* GetCurIslandTile() const { return CurIslandTile; }
	void SetCurIslandTile(class AIslandTile* NewTile) { CurIslandTile = NewTile; }
	UFUNCTION(BlueprintPure)
	class AShip* GetCurShip() const { return CurShip; }
};
