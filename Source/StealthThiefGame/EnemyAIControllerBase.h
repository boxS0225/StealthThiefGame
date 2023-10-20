// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyInterface.h"
#include "StealthThiefGameGameMode.h"
#include "EnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTHIEFGAME_API AEnemyAIControllerBase : public AAIController, public IEnemyInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> MyBehaviorTree = nullptr;

	TObjectPtr<UAISenseConfig_Sight> AISenseConfigSight = nullptr;
	TObjectPtr<UAISenseConfig_Hearing> AISenseConfigHearing = nullptr;

public:

	AEnemyAIControllerBase();

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TObjectPtr<UAISenseConfig_Sight> SetSenseSight();
	TObjectPtr<UAISenseConfig_Hearing> SetSenseHearing();

	virtual void Attack_Implementation() override;

public:

	//ツリーのゲッター・セッター・Nullチェック
	FORCEINLINE TObjectPtr<UBehaviorTree> GetMyBehaviorTree() const { return MyBehaviorTree; }
	FORCEINLINE bool SetMyBehaviorTree(const TObjectPtr<UBehaviorTree> _tree) { MyBehaviorTree = _tree; return MyBehaviorTree != nullptr; }
};
