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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBattleComponent> BattleComponent;
	
	//애니메이션 관련 변수
	UPROPERTY()
	class UUnitAnimInstance* AnimInstance;
	UPROPERTY()
	class AGuardianTile* CurTile;

private:
	UPROPERTY()
	TArray<class AShip*> EnemyShips;
	UPROPERTY()
	TArray<class ABaseTile*> AdjTiles;
	int32 RecoveryCount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	bool CheckEnemyShipAdjacentAndAttack();

private:
	void AttackEnemyShip();

public:
	UFUNCTION()
	class AGuardianTile* GetGuardianTile() const { return CurTile; }
	UFUNCTION()
	void SetGuardianTile(class AGuardianTile* NewTile);
};
