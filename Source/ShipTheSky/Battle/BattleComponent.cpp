// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/BattleComponent.h"

// Sets default values for this component's properties
UBattleComponent::UBattleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBattleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBattleComponent::SetMaxHP(float NewMaxHP, bool Recover)
{
	MaxHP = NewMaxHP;
	if (Recover) CurrentHP = MaxHP;
	CurrentHP = FMath::Clamp(CurrentHP, 0, MaxHP);
}

void UBattleComponent::TakeDamage(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP - Amount, 0, CurrentHP);
}
