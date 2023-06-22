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
	//������ ���ְ�(��)
	UPROPERTY()
	class ACommander* Commander = nullptr;

	//UI Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> TileResourcesUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> IngameMainUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> ResourceUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> PathSelectionUIClass;

	//UI
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* IngameMainUI;
	UPROPERTY()
	class UUserWidget* ResourceUI;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* PathSelectionUI;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> TileResourcesUIHolders;

	//�ڿ� UI �̹���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TArray<class UTexture2D*> ResourcesTexture;

	//���� �Է� ���� ������
	float CameraMovementSpeed;
	bool bIsPathSelectionMode;

public:
	UFUNCTION(BlueprintPure)
	class ACommander* GetCommander() const { return Commander; }

	UFUNCTION(BlueprintCallable)
	void SetIsPathSelectionMode(bool IsPathSelectionMode);
	UFUNCTION(BlueprintPure)
	bool GetIsPathSelectionMode() const { return bIsPathSelectionMode; }
	
	//UI ���� �Լ�
	UFUNCTION(BlueprintImplementableEvent)
	void OpenOwningIslandUI();
	UFUNCTION(BlueprintImplementableEvent)
	void CloseOwningIslandUI();
	UFUNCTION(BlueprintImplementableEvent)
	void OpenOwningIslandBuildingUI();
	UFUNCTION(BlueprintImplementableEvent)
	void CloseOwningIslandBuildingUI();
	UFUNCTION(BlueprintImplementableEvent)
	void OpenShipUI();
	UFUNCTION(BlueprintImplementableEvent)
	void CloseShipUI();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenPathSelectionUI();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ClosePathSelectionUI();
	UFUNCTION(BlueprintCallable)
	bool OnButtonCreateUnitPressed(enum EUnitType Type);
	UFUNCTION(BlueprintCallable)
	bool OnButtonCreateShipPressed();
	//bool OnButtonCreateShipPressed(int32 WoodCloud, int32 WoodStorm, int32 WoodSun, int32 WoodLightning, int32 WoodMeteor);
	UFUNCTION(BlueprintCallable)
	void OnButtonConstructBuildingPressed(enum EBuildingType Type);
	UFUNCTION(BlueprintCallable)
	void OnButtonUnitEmbark(class ABaseUnit* Unit);
	UFUNCTION(BlueprintCallable)
	void OnButtonUnitDisembark(class ABaseUnit* Unit);
	UFUNCTION(BlueprintCallable)
	void OnButtonDepartShip();

private:
	//���� �Է�
	virtual void SetupInputComponent() override;
	void MoveCameraHorizontal(float Value);
	void MoveCameraVertical(float Value);
	void ZoomCamera(float Value);

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void MouseRay();
};
