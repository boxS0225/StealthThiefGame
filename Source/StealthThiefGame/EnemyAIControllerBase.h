// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StealthThiefGameGameMode.h"
#include "EnemyAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTHIEFGAME_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> MyBehaviorTree;

public:

	AEnemyAIControllerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//ツリーのゲッター・セッター・Nullチェック
	FORCEINLINE TObjectPtr<UBehaviorTree> GetMyBehaviorTree() const { return MyBehaviorTree; }
	FORCEINLINE bool SetMyBehaviorTree(const TObjectPtr<UBehaviorTree> _tree) { MyBehaviorTree = _tree; return MyBehaviorTree != nullptr; }
};
