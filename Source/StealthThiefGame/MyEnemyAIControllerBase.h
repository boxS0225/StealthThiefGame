// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIControllerBase.h"
#include "MyEnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTHIEFGAME_API AMyEnemyAIControllerBase : public AEnemyAIControllerBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override {};

	virtual void OnPossess(class APawn* InPawn) override {};

	virtual void Tick(float deltatime) override {};

	// AIControllerÇÃPawnèäéùâèú
	virtual void OnUnPossess() override {};
};
