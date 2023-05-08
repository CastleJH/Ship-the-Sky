// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IslandResourceUI.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UIslandResourceUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ResourceImage1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ResourceText1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ResourceImage2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ResourceText2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ResourceImage3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ResourceText3;

public:
	UFUNCTION()
	void SetUpResourceUI1(class UTexture2D* Brush, int32 Number);
	UFUNCTION()
	void SetUpResourceUI2(class UTexture2D* Brush, int32 Number);
	UFUNCTION()
	void SetUpResourceUI3(class UTexture2D* Brush, int32 Number);
};
