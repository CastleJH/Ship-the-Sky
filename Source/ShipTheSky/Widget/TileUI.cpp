// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TileUI.h"
#include "Components/CanvasPanel.h"

void UTileUI::SetPanelVisibility(UCanvasPanel* Panel, ESlateVisibility NewVisibility)
{
	Panel->SetVisibility(NewVisibility);
}
