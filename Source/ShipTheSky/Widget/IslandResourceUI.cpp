// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/IslandResourceUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UIslandResourceUI::SetUpResourceUI1(UTexture2D* Brush, int32 Number)
{
	if (ResourceImage1 == nullptr || ResourceText1 == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Matching Resource UI"));
		return;
	}
	if (Brush)
	{
		ResourceImage1->SetBrushFromTexture(Brush);
		ResourceText1->SetText(FText::FromString(FString::Printf(TEXT("%d"), Number)));
	}
	else
	{
		ResourceImage1->SetVisibility(ESlateVisibility::Hidden);
		ResourceText1->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UIslandResourceUI::SetUpResourceUI2(UTexture2D* Brush, int32 Number)
{
	if (ResourceImage2 == nullptr || ResourceText2 == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Matching Resource UI"));
		return;
	}
	if (Brush)
	{
		ResourceImage2->SetBrushFromTexture(Brush);
		ResourceText2->SetText(FText::FromString(FString::Printf(TEXT("%d"), Number)));
	}
	else
	{
		ResourceImage2->SetVisibility(ESlateVisibility::Hidden);
		ResourceText2->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UIslandResourceUI::SetUpResourceUI3(UTexture2D* Brush, int32 Number)
{
	if (ResourceImage3 == nullptr || ResourceText3 == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Matching Resource UI"));
		return;
	}
	if (Brush)
	{
		ResourceImage3->SetBrushFromTexture(Brush);
		ResourceText3->SetText(FText::FromString(FString::Printf(TEXT("%d"), Number)));
	}
	else
	{
		ResourceImage3->SetVisibility(ESlateVisibility::Hidden);
		ResourceText3->SetVisibility(ESlateVisibility::Hidden);
	}
}
