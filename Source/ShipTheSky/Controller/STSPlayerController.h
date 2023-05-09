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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> IslandTileUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> ResourceUIClass;

private:
	class ACommander* Commander = nullptr;
	class UIslandTileUI* IslandTileUI;
	class UResourceUI* ResourceUI;
	float CameraMovementSpeed;

public:
	class ACommander* GetCommander() { return Commander; }
	UFUNCTION(BlueprintCallable)
	void OnButtonCreateUnitPressed(enum EUnitType Type);
	UFUNCTION(BlueprintCallable)
	void OnButtonConstructBuildingPressed(enum EBuildingType Type);
	UFUNCTION(BlueprintCallable)
	void OnButtonGenerateMap();

protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	void MoveCameraHorizontal(float Value);
	void MoveCameraVertical(float Value);
	void ZoomCamera(float Value);
};
