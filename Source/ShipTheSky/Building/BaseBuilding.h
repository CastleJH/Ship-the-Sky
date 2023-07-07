// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBuilding.generated.h"

UCLASS()
class SHIPTHESKY_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABaseBuilding();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Building Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	//건물 관련 변수
	UPROPERTY(VisibleAnywhere, Category = "Building")
	EBuildingType BuildingType;
	UPROPERTY(VisibleAnywhere, Category = "Building")
	class AIslandTile* CurTile;

	//건물의 작업 관련 변수
	int32 Progress;

	UPROPERTY(VisibleAnywhere)
	class ACommander* OwnerCommander;

public:
	UFUNCTION()
	void SetOwnerCommander(class ACommander* NewCommnader) { OwnerCommander = NewCommnader; }
	UFUNCTION(BlueprintPure)
	class ACommander* GetOwnerCommander() const { return OwnerCommander; }

public:
	//건물 관련 변수 접근
	UFUNCTION(BlueprintPure)
	EBuildingType GetBuildingType() const { return BuildingType; }
	class AIslandTile* GetCurTile() const { return CurTile; }
	void SetCurTile(class AIslandTile* NewTile) { CurTile = NewTile; };

	//작업 진행
	virtual void IncreaseProgress() { Progress++; }
	void ResetProgress() { Progress = 0; }
	int32 GetProgress() const { return Progress; }
};