// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Miner.h"
#include "Battle/BattleComponent.h"

AMiner::AMiner()
{
	UnitType = EUnitType::Miner;

	Efficiency = 0.0f;
	FoodConsume = 0.5f;
	BattleComponent->SetDamage(10);
	BattleComponent->SetMaxHP(100, true);
}
