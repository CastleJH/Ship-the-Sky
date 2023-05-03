// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ResourceUI.h"
#include "Components/TextBlock.h"
#include "Pawn/Commander.h"


void UResourceUI::SetResourceText(int32 Amount, EResourceType Resource)
{
	switch (Resource)
	{
	case EResourceType::StoneCloud:
		StoneCloudText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::StoneStorm:
		StoneStormText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::StoneSun:
		StoneSunText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::StoneLightning:
		StoneLightningText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::StoneMeteor:
		StoneMeteorText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::WoodCloud:
		WoodCloudText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::WoodStorm:
		WoodStormText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::WoodSun:
		WoodSunText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::WoodLightning:
		WoodLightningText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::WoodMeteor:
		WoodMeteorText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	case EResourceType::Food:
		FoodText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Amount)));
		break;
	default:
		break;
	}
}
