// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "BaseUnit.generated.h"


UCLASS()
class SHIPTHESKY_API ABaseUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseUnit();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	
	//���� ���� ����
	EUnitType UnitType;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class AIslandTile* CurIslandTile;
	UPROPERTY(VisibleAnywhere, Category = "Unit")
	class AShip* CurShip;

public:
	//���ֿ��� ���� �� �ִ� ���
	void LocateOnIslandTile(class AIslandTile* Tile, bool bIsImmediate);
	bool Embark(class AShip* Ship);
	bool Disembark();

	//���� ���� ���� ����
	UFUNCTION(BlueprintPure)
	EUnitType GetUnitType() const { return UnitType; }
	UFUNCTION(BlueprintPure)
	class AIslandTile* GetCurIslandTile() const { return CurIslandTile; }
	void SetCurIslandTile(class AIslandTile* NewTile) { CurIslandTile = NewTile; }
	UFUNCTION(BlueprintPure)
	class AShip* GetCurShip() const { return CurShip; }

private:
	UPROPERTY(VisibleAnywhere)
	TArray<class ABaseTile*> Path;
	float MoveSpeed;

	virtual void Tick(float DeltaSeconds) override;
	void MakePath(class ABaseTile* From, class ABaseTile* To);
};
