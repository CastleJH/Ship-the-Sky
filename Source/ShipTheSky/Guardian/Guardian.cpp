// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian/Guardian.h"
#include "Battle/BattleComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AGuardian::AGuardian()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent0"));
	SkeletalMeshComponent->SetupAttachment(Root);
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	BattleComponent = CreateDefaultSubobject<UBattleComponent>(TEXT("BattleComponent"));

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Tile UI"));

	WidgetComp->SetupAttachment(RootComponent);
	WidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
	WidgetComp->SetDrawSize(FVector2D(100.0f, 10.0f));
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetTickMode(ETickMode::Automatic);
	WidgetComp->SetWidgetClass(LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/GuardianUI/WBP_GuardianHPBar.WBP_GuardianHPBar_C")));
}

// Called when the game starts or when spawned
void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP(), true);
}