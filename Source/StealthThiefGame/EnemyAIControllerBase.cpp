// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIControllerBase.h"

AEnemyAIControllerBase::AEnemyAIControllerBase()
{
	SetGenericTeamId(FGenericTeamId(1));
}

void AEnemyAIControllerBase::BeginPlay()
{
	if (GetMyBehaviorTree() == nullptr) { return; }
	RunBehaviorTree(GetMyBehaviorTree());
}
