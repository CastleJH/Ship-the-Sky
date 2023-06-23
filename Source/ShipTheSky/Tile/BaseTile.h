// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "BaseTile.generated.h"


UCLASS()
class SHIPTHESKY_API ABaseTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTile();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Tile Mesh", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Widget", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> TileWidgetComp;

	//타일 속성 관련 변수
	UPROPERTY(VisibleAnywhere, Category = "Tile")
	ETileType TileType;
	UPROPERTY()
	class AShip* ShipOnThisTile = nullptr;
	int32 Row;
	int32 Col;

public:
	//타일이 일정 시간마다 실행할 함수
	virtual void TimePass(int32 GameDate);

	//타일 속성 관련 변수 접근
	ETileType GetTileType() const { return TileType; }
	virtual void SetShip(class AShip* NewShip) { ShipOnThisTile = NewShip; }
	UFUNCTION(BlueprintPure)
	class AShip* GetShip() const { return ShipOnThisTile; }
	void SetRow(int32 NewRow) { Row = NewRow; }
	int32 GetRow() const { return Row; }
	void SetCol(int32 NewCol) { Col = NewCol; }
	int32 GetCol() const { return Col; }

	//임시
	UFUNCTION(BlueprintCallable)
	virtual void OnTileSelectedAsView(class ASTSPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable)
	bool OnTileFirstSelectedAsPath(class ASTSPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable)
	void OnTileSelectedAsPath(class ASTSPlayerController* PlayerController);
	UFUNCTION()
	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComp; }

protected:
	virtual void BeginPlay() override;
};
