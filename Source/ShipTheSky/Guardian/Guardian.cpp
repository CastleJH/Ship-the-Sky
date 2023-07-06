// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian/Guardian.h"
#include "Battle/BattleComponent.h"

// Sets default values
AGuardian::AGuardian()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent0"));
	SkeletalMeshComponent->SetupAttachment(Root);

	BattleComponent = CreateDefaultSubobject<UBattleComponent>(TEXT("BattleComponent"));
}

// Called when the game starts or when spawned
void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	
	BattleComponent->SetDamage(FMath::RandRange(10, 30));
	BattleComponent->SetMaxHP(FMath::RandRange(50, 100), true);
}

// Called every frame
void AGuardian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}