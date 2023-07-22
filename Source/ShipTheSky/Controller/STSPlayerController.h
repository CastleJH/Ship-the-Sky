// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Enums.h"
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
	//보유한 지휘관(폰)
	UPROPERTY()
	class ACommander* Commander = nullptr;
	UPROPERTY()
	class APlayerCommander* PlayerCommander;

	//UI 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> IngameUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> TileResourceUIClass;

	//UI
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* IngameUI;

	//자원 UI 이미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TArray<class UTexture2D*> ResourcesTexture;

	//유저 입력 관련 변수들
	float CameraMovementSpeed;
	float CameraZoomSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EUserInputMode UserInputMode;
	bool bIsPathSelectionValid;
	bool bIsUnitRelocationMode;

	//비행선 고정 모드
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AShip* LockedShip;

	//기타 내부적으로 쓰이는 변수들
	UPROPERTY()
	class AShip* PrevShip;
	UPROPERTY()
	class AIslandTile* FirstTile;
	UPROPERTY()
	class ABaseUnit* UnitWaitingRelocationFromUI;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AShip* RemovingShipTarget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ABaseUnit* RemovingUnitTarget;
	float PrevArmLength;

public:
	UFUNCTION(BlueprintPure)
	class ACommander* GetCommander() const { return Commander; }

	//인풋 변경
	UFUNCTION(BlueprintCallable)
	void SetToViewMode();
	UFUNCTION(BlueprintCallable)
	void SetToPathSelectionMode();
	UFUNCTION(BlueprintCallable)
	void SetToPortalSelectionMode();

	//인풋 모드 확인
	EUserInputMode GetUserInputMode() const { return UserInputMode; }

	//UI 버튼 눌릴 때
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
	UFUNCTION(BlueprintCallable)
	void OnButtonLocateUnitOnTile(class ABaseUnit* Unit);
	UFUNCTION(BlueprintCallable)
	void OnButtonStartPortal();
	UFUNCTION(BlueprintCallable)
	void OnButtonCancelPortal();
	UFUNCTION(BlueprintCallable)
	void OnButtonRemoveBuilding();
	UFUNCTION(BlueprintCallable)
	void OnButtonRemoveShip();
	UFUNCTION(BlueprintCallable)
	void OnButtonRemoveUnit();

private:
	//유저 입력
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputMappingContext> ViewTileMode;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputMappingContext> PathSelectionMode;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputMappingContext> PortalSelectionMode;
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
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> InputMouseReleasedForPortal;

	virtual void SetupInputComponent() override;

	void MoveCamera(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);
	void MousePressedForReloc(const FInputActionValue& Value);
	void MouseReleased(const FInputActionValue& Value);
	void MousePressedForPath(const FInputActionValue& Value);
	void MouseDraggedForPath(const FInputActionValue& Value);
	void MouseReleasedForPortal(const FInputActionValue& Value);

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	class ABaseTile* MouseRay();
public:
	virtual void Tick(float DeltaSeconds) override;

//나중에 다른 곳으로 옮겨
public:
	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TArray<TSubclassOf<class AGuardian>> GuardianClass;
};
