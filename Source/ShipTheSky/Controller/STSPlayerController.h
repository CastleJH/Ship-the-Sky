// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API ASTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTSPlayerController();

private:
	float CameraMovementSpeed;

protected:
	virtual void SetupInputComponent() override;

private:
	void MoveCameraHorizontal(float Value);
	void MoveCameraVertical(float Value);
	void ZoomCamera(float Value);
};
