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

protected:
	//UPROPERTY(VisibleAnywhere, Category = UI)
	//class UWidgetComponent* WidgetComponent;
	//UPROPERTY(EditDefaultsOnly, Category = "Widget")
	//TSubclassOf<class UUserWidget> IslandResourceUIClass;

protected:
	//class UIslandResourceUI* IslandResourceUI;

	UPROPERTY(VisibleAnywhere)
	int32 IslandID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EIslandTileType IslandType;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetIslandID() const { return IslandID; }
	void SetIslandID(int32 NewIslandID) { IslandID = NewIslandID; }

	EIslandTileType GetIslandType() const { return IslandType; };

	void OnTileReleased(AActor* Target, FKey ButtonPressed) override;

protected:
	virtual void BeginPlay() override;
};
