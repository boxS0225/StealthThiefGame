// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovePoints.generated.h"

UCLASS()
class STEALTHTHIEFGAME_API AMovePoints : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Root, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = Points, Meta = (MakeEditWidget = true))
	TArray<FVector> MovePoints;

public:	
	// Sets default values for this actor's properties
	AMovePoints();

protected:

public:	

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetMovePoints() const { return MovePoints; }

};
