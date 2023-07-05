// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/BaseTile.h"
#include "Enums.h"
#include "IslandTile.generated.h"

/**
 * 
 */

UCLASS()
class SHIPTHESKY_API AIslandTile : public ABaseTile
{
	GENERATED_BODY()
	
public:
	AIslandTile();

protected:
	//섬 관련 변수
	UPROPERTY(VisibleAnywhere)
	int32 IslandID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EIslandTileType IslandType;

public:
	//섬 관련 변수 접근
	UFUNCTION(BlueprintPure)
	int32 GetIslandID() const { return IslandID; }
	void SetIslandID(int32 NewIslandID) { IslandID = NewIslandID; }
	EIslandTileType GetIslandType() const { return IslandType; };
	UFUNCTION(BlueprintPure)
	class AGuardianTile* GetGuardianTile() const;
	
	//임시
	virtual void OnTileSelectedAsView(class ASTSPlayerController* PlayerController) override;

protected:
	virtual void BeginPlay() override;
};
