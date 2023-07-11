// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Guardian.generated.h"

UCLASS()
class SHIPTHESKY_API AGuardian : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuardian();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBattleComponent> BattleComponent;
	class AGuardianTile* Tile;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "Widget", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> WidgetComp;
	
	//�ִϸ��̼� ���� ����
	UPROPERTY()
	class UUnitAnimInstance* AnimInstance;

	//���� ���� ����
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 ScorePower;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 ScorePowerUpgradeDelta;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 HPUpgradeDelta;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 AttackUpgradeDelta;
	int32 HPCloudLevel;
	int32 HPSunLevel;
	int32 AttackStormLevel;
	int32 AttackLightningLevel;
	int32 ScorePowerLevel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//���� ����
	UFUNCTION(BlueprintPure)
	int32 GetScorePower() { return ScorePower; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeHPWithCloud();
	UFUNCTION(BlueprintPure)
	int32 GetHPUpgradeCostWithCloud() const { return HPCloudLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeHPWithSun();
	UFUNCTION(BlueprintPure)
	int32 GetHPUpgradeCostWithSun() const { return HPSunLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeAttackWithStorm();
	UFUNCTION(BlueprintPure)
	int32 GetAttackUpgradeCostWithStorm() const { return AttackStormLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeAttackWithLightning();
	UFUNCTION(BlueprintPure)
	int32 GetAttackUpgradeCostWithLightning() const { return AttackLightningLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeScorePower();
	UFUNCTION(BlueprintPure)
	int32 GetScorePowerUpgradeCost() const { return ScorePowerLevel * 10; }
	void ResetLevelAndPower();
	UFUNCTION(BlueprintPure)
	bool IsReinforcementPanelVisible() const;
};
