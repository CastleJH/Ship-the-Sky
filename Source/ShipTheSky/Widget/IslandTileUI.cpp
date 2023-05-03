// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/IslandTileUI.h"
#include "Components/CanvasPanel.h"

void UIslandTileUI::SetPanelVisibility(UCanvasPanel* Panel, ESlateVisibility NewVisibility)
{
	Panel->SetVisibility(NewVisibility);
}

