// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Farmer.h"
#include "Battle/BattleComponent.h"

AFarmer::AFarmer()
{
	UnitType = EUnitType::Farmer;

	Efficiency = 0.0f;
	FoodConsume = 0.5f;
	BattleComponent->SetDamage(10);
	BattleComponent->SetMaxHP(100, true);
}
