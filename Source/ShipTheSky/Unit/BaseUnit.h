// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseUnit.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Miner = 0,
	Woodcutter,
	Farmer,
	Warrior,
	None
};

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
	class AIslandTile* CurTile;

public:
	void LocateToResourceTile(class AResourceTile* ResourceTile);

	UFUNCTION(BlueprintPure)
	EUnitType GetUnitType() const { return UnitType; }
	class AIslandTile* GetCurTile() const { return CurTile; }
	void SetCurTile(class AIslandTile* NewTile) { CurTile = NewTile; }
};
