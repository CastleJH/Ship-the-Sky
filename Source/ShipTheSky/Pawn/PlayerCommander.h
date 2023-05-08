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

private:
	UPROPERTY(VisibleAnywhere, Category = "Tile Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
