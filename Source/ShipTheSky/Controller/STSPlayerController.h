// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
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
	UPROPERTY()
	class APlayerCommander* PlayerCommander;

	//UI Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> IngameUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> TileResourceUIClass;

	//UI
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* IngameUI;

	//�ڿ� UI �̹���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TArray<class UTexture2D*> ResourcesTexture;

	//���� �Է� ���� ������
	float CameraMovementSpeed;
	float CameraZoomSpeed;
	bool bIsPathSelectionMode;
	bool bIsPathSelectionValid;

	//���༱ ���� ���
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AShip* LockedShip;

	//��Ÿ ���������� ���̴� ������
	UPROPERTY()
	class AShip* PrevShip;
	class AIslandTile* FirstTile;

public:
	UFUNCTION(BlueprintPure)
	class ACommander* GetCommander() const { return Commander; }

	//��� ���� ���
	UFUNCTION(BlueprintCallable)
	void SetIsPathSelectionMode(bool IsPathSelectionMode);
	UFUNCTION(BlueprintPure)
	bool GetIsPathSelectionMode() const { return bIsPathSelectionMode; }	

	//UI ��ư ���� ��
	UFUNCTION(BlueprintCallable)
	bool OnButtonCreateUnitPressed(enum EUnitType Type);
	UFUNCTION(BlueprintCallable)
	bool OnButtonCreateShipPressed();
	UFUNCTION(BlueprintCallable)
	void OnButtonConstructBuildingPressed(enum EBuildingType Type);
	UFUNCTION(BlueprintCallable)
	void OnButtonUnitEmbark(class ABaseUnit* Unit);
	UFUNCTION(BlueprintCallable)
	void OnButtonUnitDisembark(class ABaseUnit* Unit);
	UFUNCTION(BlueprintCallable)
	void OnButtonDepartShip();
	UFUNCTION(BlueprintCallable)
	void OnButtonStopShip();
	UFUNCTION(BlueprintCallable)
	void OnButtonLookTile(class ABaseTile* Tile);

private:
	//���� �Է�
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputMappingContext> TileSelectionMode;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputMappingContext> PathSelectionMode;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputMove;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputZoom;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputMousePressedForReloc;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputMouseReleased;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputMousePressedForPath;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputMouseDraggedForPath;

	virtual void SetupInputComponent() override;

	void MoveCamera(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);
	void MousePressedForeReloc(const FInputActionValue& Value);
	void MouseReleased(const FInputActionValue& Value);
	void MousePressedForPath(const FInputActionValue& Value);
	void MouseDraggedForPath(const FInputActionValue& Value);

	UFUNCTION()
	void RelocateUnitOnTwoTile(class AIslandTile* Tile1, class AIslandTile* Tile2);

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	class ABaseTile* MouseRay();
public:
	virtual void Tick(float DeltaSeconds) override;
};
