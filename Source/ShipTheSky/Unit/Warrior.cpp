// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Warrior.h"
#include "Battle/BattleComponent.h"

AWarrior::AWarrior()
{
	UnitType = EUnitType::Warrior;

	Efficiency = 20.0f;
	FoodConsume = 0.5f;
	BattleComponent->SetDamage(20);
	BattleComponent->SetMaxHP(200, true);
}
