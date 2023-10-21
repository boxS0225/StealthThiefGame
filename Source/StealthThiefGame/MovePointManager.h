// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MovePointManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHTHIEFGAME_API UMovePointManager : public USceneComponent
{
	GENERATED_BODY()

	//à⁄ìÆâ¬î\ç¿ïW
	UPROPERTY(EditAnywhere, Category = Points, Meta = (MakeEditWidget = true))
	TArray<FVector> MovePoints;

	int currentPointIndex = 0;

public:	
	// Sets default values for this component's properties
	UMovePointManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetMovePoints() const { return MovePoints; }

	UFUNCTION(BlueprintCallable)
	FVector GetMovePoint(const int _num) const { return MovePoints[_num]; }

	UFUNCTION(BlueprintCallable)
	int GetPointIndex() const { return currentPointIndex; }

	UFUNCTION(BlueprintCallable)
	void SetPointIndex(const int _num)  { currentPointIndex = _num; }
};
