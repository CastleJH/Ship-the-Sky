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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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
	float TakeDamage(float Amount);
	void RecoverHP(float Amount);

	bool IsAlive() { return CurrentHP > KINDA_SMALL_NUMBER; }
};
