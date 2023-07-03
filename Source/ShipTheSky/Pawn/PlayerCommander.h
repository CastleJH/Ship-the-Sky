// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/Commander.h"
#include "PlayerCommander.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API APlayerCommander : public ACommander
{
	GENERATED_BODY()

public:
	APlayerCommander();

	void MoveCommanderToTile(class ABaseTile* Tile, bool bSelectTileToo);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComp;

	float SpringArmLength;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
