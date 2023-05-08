// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBuilding.generated.h"

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Barracks = 0,
	Shipyard,
	Portal,
	Sanctuary
};

UCLASS()
class SHIPTHESKY_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABaseBuilding();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Building Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Building")
	EBuildingType BuildingType;
	UPROPERTY(VisibleAnywhere, Category = "Building")
	class AIslandTile* CurTile;
	int32 Progress;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EBuildingType GetBuildingType() const { return BuildingType; }
	class AIslandTile* GetCurTile() const { return CurTile; }
	void SetCurTile(class AIslandTile* NewTile) { CurTile = NewTile; };

	virtual void IncreaseProgress() { Progress++; }
	void ResetProgress() { Progress = 0; }
	int32 GetProgress() const { return Progress; }
};
