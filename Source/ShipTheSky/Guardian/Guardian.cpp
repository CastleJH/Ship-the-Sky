// Fill out your copyright notice in the Description page of Project Settings.


#include "Guardian/Guardian.h"
#include "Battle/BattleComponent.h"
#include "Components/WidgetComponent.h"
#include "Tile/GuardianTile.h"
#include "Pawn/Commander.h"

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

	HPCloudLevel = 1;
	HPSunLevel = 1;
	AttackStormLevel = 1;
	AttackLightningLevel = 1;
	ScorePowerLevel = 1;
}

// Called when the game starts or when spawned
void AGuardian::BeginPlay()
{
	Super::BeginPlay();
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP(), true);
}

bool AGuardian::UpgradeHPWithCloud()
{
	if (Tile->GetIslandOwner()->GetResource(EResourceType::StoneCloud) < GetHPUpgradeCostWithCloud()) return false;

	Tile->GetIslandOwner()->SetResource(Tile->GetIslandOwner()->GetResource(EResourceType::StoneCloud) - GetHPUpgradeCostWithCloud(), EResourceType::StoneCloud);
	HPCloudLevel++;
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP() + HPUpgradeDelta, false);
	return true;
}

bool AGuardian::UpgradeHPWithSun()
{
	if (Tile->GetIslandOwner()->GetResource(EResourceType::StoneSun) < GetHPUpgradeCostWithSun()) return false;

	Tile->GetIslandOwner()->SetResource(Tile->GetIslandOwner()->GetResource(EResourceType::StoneSun) - GetHPUpgradeCostWithSun(), EResourceType::StoneSun);
	HPSunLevel++;
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP() + HPUpgradeDelta, false);
	return true;
}

bool AGuardian::UpgradeAttackWithStorm()
{
	if (Tile->GetIslandOwner()->GetResource(EResourceType::StoneStorm) < GetAttackUpgradeCostWithStorm()) return false;

	Tile->GetIslandOwner()->SetResource(Tile->GetIslandOwner()->GetResource(EResourceType::StoneStorm) - GetAttackUpgradeCostWithStorm(), EResourceType::StoneStorm);
	AttackStormLevel++;
	BattleComponent->SetDamage(BattleComponent->GetDamage() + AttackUpgradeDelta);
	return true;
}

bool AGuardian::UpgradeAttackWithLightning()
{
	if (Tile->GetIslandOwner()->GetResource(EResourceType::StoneLightning) < GetAttackUpgradeCostWithLightning()) return false;

	Tile->GetIslandOwner()->SetResource(Tile->GetIslandOwner()->GetResource(EResourceType::StoneLightning) - GetAttackUpgradeCostWithLightning(), EResourceType::StoneLightning);
	AttackLightningLevel++;
	BattleComponent->SetDamage(BattleComponent->GetDamage() + AttackUpgradeDelta);
	return true;
}

bool AGuardian::UpgradeScorePower()
{
	if (Tile->GetIslandOwner()->GetResource(EResourceType::StoneMeteor) < GetScorePowerUpgradeCost()) return false;

	Tile->GetIslandOwner()->SetResource(Tile->GetIslandOwner()->GetResource(EResourceType::StoneMeteor) - GetScorePowerUpgradeCost(), EResourceType::StoneMeteor);
	ScorePowerLevel++;
	ScorePower += ScorePowerUpgradeDelta;
	return true;
}

void AGuardian::ResetLevelAndPower()
{
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP() - HPUpgradeDelta * (HPCloudLevel - 1), false);
	HPCloudLevel = 1;
	BattleComponent->SetMaxHP(BattleComponent->GetMaxHP() - HPUpgradeDelta * (HPSunLevel - 1), false);
	HPSunLevel = 1;
	BattleComponent->SetDamage(BattleComponent->GetDamage() - AttackUpgradeDelta * (AttackStormLevel - 1));
	AttackStormLevel = 1;
	BattleComponent->SetDamage(BattleComponent->GetDamage() - AttackUpgradeDelta * (AttackLightningLevel - 1));
	AttackLightningLevel = 1;
	ScorePower -= ScorePowerUpgradeDelta * (ScorePowerLevel - 1);
	ScorePowerLevel = 1;
}

bool AGuardian::IsReinforcementPanelVisible() const
{
	return Tile->IsBuildingTypeBuilt(EBuildingType::Sanctuary) && Tile->GetIslandOwner() == Cast<ACommander>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
