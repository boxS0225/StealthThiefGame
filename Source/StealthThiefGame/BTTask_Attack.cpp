// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyInterface.h"
#include "AIController.h"
#include "BTTask_Attack.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.Implements<UEnemyInterface>())
	{
		AAIController* aIOwner = OwnerComp.GetAIOwner(); // to get the controller
		APawn* pawn = aIOwner->GetPawn();

		//ƒLƒƒƒ‰‚²‚Æ‚ÌUŒ‚‚ğ‚·‚é
		IEnemyInterface::Execute_Attack(pawn);
	}

	return EBTNodeResult::Type();
}
