// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/PlayerCommander.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerCommander::APlayerCommander()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = CameraComp;
}

void APlayerCommander::BeginPlay()
{
	Super::BeginPlay();
	CameraComp->SetRelativeRotation(FRotator(290.0f, 0.0f, 0.0f));
}
