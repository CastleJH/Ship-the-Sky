// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "BaseUnit.generated.h"


UCLASS()
class SHIPTHESKY_API ABaseUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseUnit();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBattleComponent> BattleComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	//유닛 관련 변수
	EUnitType UnitType;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class AIslandTile* CurIslandTile;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class AShip* CurShip;

	//스탯 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FoodConsume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Efficiency;
	UPROPERTY(VisibleAnywhere)
	int32 HPLevel;
	UPROPERTY(VisibleAnywhere)
	int32 FoodConsumeLevel;
	UPROPERTY(VisibleAnywhere)
	int32 AttackLevel;
	UPROPERTY(VisibleAnywhere)
	int32 EfficiencyLevel;
	UPROPERTY(VisibleAnywhere)
	int32 AllUpLevel;

	//애니메이션 관련 변수
	UPROPERTY()
	class UUnitAnimInstance* AnimInstance;

	//소유자
	UPROPERTY(VisibleAnywhere)
	class ACommander* OwnerCommander;

public:
	//유닛에게 들어올 수 있는 명령
	void LocateOnIslandTile(class AIslandTile* Tile, bool bIsImmediate);
	bool Embark(class AShip* Ship);
	bool Disembark();

	//유닛 관련 변수 접근
	UFUNCTION(BlueprintPure)
	EUnitType GetUnitType() const { return UnitType; }
	UFUNCTION(BlueprintPure)
	class AIslandTile* GetCurIslandTile() const { return CurIslandTile; }
	void SetCurIslandTile(class AIslandTile* NewTile) { CurIslandTile = NewTile; }
	UFUNCTION(BlueprintPure)
	class AShip* GetCurShip() const { return CurShip; }

	//스탯 관련
	UFUNCTION(BlueprintPure)
	float GetFoodConsume() const { return FoodConsume; }
	UFUNCTION(BlueprintPure)
	float GetEfficiency() const { return Efficiency; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeHP();
	UFUNCTION(BlueprintPure)
	int32 GetHPUpgradeCost() const { return 40 + HPLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeFoodConsume();
	UFUNCTION(BlueprintPure)
	int32 GetFoodConsumeUpgradeCost() const { return 40 + FoodConsumeLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeAttack();
	UFUNCTION(BlueprintPure)
	int32 GetAttackUpgradeCost() const { return 40 + AttackLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeEfficiency();
	UFUNCTION(BlueprintPure)
	int32 GetEfficiencyUpgradeCost() const { return 40 + EfficiencyLevel * 10; }
	UFUNCTION(BlueprintCallable)
	bool UpgradeAll();
	UFUNCTION(BlueprintPure)
	int32 GetAllUpgradeCost() const { return 40 + AllUpLevel * 10; }
	TPair<enum EUnitStat, int32> GetStatUpgradeRecommendation();
	
	//전투 관련
	float GetAttacked(float Damage);

	//소유자
	UFUNCTION()
	void SetOwnerCommander(class ACommander* NewCommnader) { OwnerCommander = NewCommnader; }
	UFUNCTION(BlueprintPure)
	class ACommander* GetOwnerCommander() const { return OwnerCommander; }

private:
	UPROPERTY(VisibleAnywhere)
	TArray<class ABaseTile*> Path;
	float MoveSpeed;

	virtual void Tick(float DeltaSeconds) override;
	void MakePath(class ABaseTile* From, class ABaseTile* To);

	virtual void BeginPlay() override;
};
