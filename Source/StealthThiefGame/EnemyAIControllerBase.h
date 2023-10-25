// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StealthThiefGameGameMode.h"
#include "EnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTHIEFGAME_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> MyBehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;

	UBlackboardComponent* BlackboardComp;


	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> AISenseConfigSight = nullptr;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> AISenseConfigHearing = nullptr;

public:

	AEnemyAIControllerBase();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float deltatime) override;

	// AIControllerのPawn所持
	virtual void OnPossess(class APawn* InPawn) override;

	// AIControllerのPawn所持解除
	virtual void OnUnPossess() override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	TObjectPtr<UAISenseConfig_Sight> SetSenseSight();
	TObjectPtr<UAISenseConfig_Hearing> SetSenseHearing();

public:
	//ツリーのゲッター・セッター・Nullチェック
	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetMyBehaviorTree() const { return MyBehaviorTree; }
	FORCEINLINE bool SetMyBehaviorTree(const TObjectPtr<UBehaviorTree> _tree) { MyBehaviorTree = _tree; return MyBehaviorTree != nullptr; }
	FORCEINLINE TObjectPtr<UBlackboardData> GetBlackboard() const { return GetMyBehaviorTree()->BlackboardAsset; }
};
