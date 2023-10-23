// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController.h"
#include "BTTask_GetMovePoint.h"

void UBTTask_GetMovePoint::SetBlackBoardValue(UMovePointManager* _manager, UBlackboardComponent* _blackboard)
{
	_blackboard->SetValueAsVector(GetSelectedBlackboardKey(), _manager->GetCurrentPoint());
}

EBTNodeResult::Type UBTTask_GetMovePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* aIOwner = OwnerComp.GetAIOwner();
	APawn* pawn = aIOwner->GetPawn();

	UMovePointManager* manager = pawn->GetComponentByClass<UMovePointManager>();

	if (manager == nullptr) { return EBTNodeResult::Failed; }

	SetBlackBoardValue(manager, myBlackboard);

	//次の目的地をセット
	manager->SetNextPoint();

	return EBTNodeResult::Succeeded;
}