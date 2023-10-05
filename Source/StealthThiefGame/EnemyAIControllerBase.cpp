// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIControllerBase.h"
#include "StealthThiefGameGameMode.h"

AEnemyAIControllerBase::AEnemyAIControllerBase()
{
	SetGenericTeamId(FGenericTeamId(1));
}

void AEnemyAIControllerBase::BeginPlay()
{
	TObjectPtr<UBehaviorTree> tree = GetMyBehaviorTree();

	//ツリーのNULLチェック
	AStealthThiefGameGameMode::CheckPointerContent<UBehaviorTree>(tree);
	//ツリーを起動
	RunBehaviorTree(tree);
}
