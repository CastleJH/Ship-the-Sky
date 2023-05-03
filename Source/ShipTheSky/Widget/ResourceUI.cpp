// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ResourceUI.h"
#include "Components/TextBlock.h"
#include "Pawn/PlayerCommander.h"


void UResourceUI::SetResourceText(EResourceType Resource)
{
	if (PlayerCommander == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Commander"));
		return;
	}
	switch (Resource)
	{
	case EResourceType::StoneCloud:
		StoneCloudText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::StoneStorm:
		StoneStormText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::StoneSun:
		StoneSunText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::StoneLightning:
		StoneLightningText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::StoneMeteor:
		StoneMeteorText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::WoodCloud:
		WoodCloudText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::WoodStorm:
		WoodStormText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::WoodSun:
		WoodSunText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::WoodLightning:
		WoodLightningText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::WoodMeteor:
		WoodMeteorText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	case EResourceType::Food:
		FoodText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCommander->GetResource(Resource))));
		break;
	default:
		break;
	}
}

