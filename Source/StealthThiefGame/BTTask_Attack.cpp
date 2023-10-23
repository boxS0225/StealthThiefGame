// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyInterface.h"
#include "AIController.h"
#include "BTTask_Attack.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* aIOwner = OwnerComp.GetAIOwner();
	APawn* pawn = aIOwner->GetPawn();

	if (pawn->Implements<UEnemyInterface>())
	{
		//�L�������Ƃ̍U��������
		IEnemyInterface::Execute_Attack(pawn);
	}

	return EBTNodeResult::Type();
}
