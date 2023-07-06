// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPTHESKY_API UBattleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBattleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//½ºÅÈ º¯¼ö
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Damage;

public:
	UFUNCTION(BlueprintPure)
	float GetDamage() const { return Damage; }
	UFUNCTION(BlueprintPure)
	float GetMaxHP() const { return MaxHP; }
	UFUNCTION(BlueprintPure)
	float GetCurHP() const { return CurrentHP; }

	void SetMaxHP(float NewMaxHP, bool Recover);
	void SetDamage(float NewDamage) { Damage = NewDamage; }
	void TakeDamage(float Amount);
	void RecoverHP();

	bool IsAlive() { return CurrentHP != 0; }
};
