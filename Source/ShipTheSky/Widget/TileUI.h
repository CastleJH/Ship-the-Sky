// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileUI.generated.h"

/**
 * 
 */
UCLASS()
class SHIPTHESKY_API UTileUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* ConstructionPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* BarracksPanel;

	void SetPanelVisibility(UCanvasPanel* Panel, enum ESlateVisibility NewVisibility);
};
