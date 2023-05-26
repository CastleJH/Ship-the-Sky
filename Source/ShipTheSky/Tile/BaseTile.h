// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTile.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Island = 0,
	Cloud,
	Storm,
	Sun,
	Lightning,
	Meteor,
	End
};

UCLASS()
class SHIPTHESKY_API ABaseTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTile();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Tile Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	ETileType TileType;

	int32 Row;
	int32 Col;

private:
	UPROPERTY()
	class AShip* ShipOnThisTile = nullptr;

public:
	ETileType GetTileType() const { return TileType; }
	virtual void TimePass(int32 GameDate);

	void SetRow(int32 NewRow) { Row = NewRow; }
	int32 GetRow() const { return Row; }
	void SetCol(int32 NewCol) { Col = NewCol; }
	int32 GetCol() const { return Col; }

	UFUNCTION(BlueprintCallable)
	virtual void OnTileReleased(AActor* Target, FKey ButtonPressed);

	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComp; }

	void SetShip(class AShip* NewShip) { ShipOnThisTile = NewShip; }
	UFUNCTION(BlueprintPure)
	class AShip* GetShip() const { return ShipOnThisTile; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
