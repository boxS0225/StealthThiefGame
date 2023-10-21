// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController.h"
#include "BTTask_GetMovePoint.h"

void UBTTask_GetMovePoint::SetNextPath(UMovePointManager* _manager, int _length)
{
	//今の場所と配列の取得
	int index = _manager->GetPointIndex();

	//インクリメント
	index = index + 1 < _length ? index + 1 : 0;

	_manager->SetPointIndex(index);
}

void UBTTask_GetMovePoint::SetBlackBoardValue(UMovePointManager* _manager, UBlackboardComponent* _blackboard)
{
	int index = _manager->GetPointIndex();

	_blackboard->SetValueAsVector(TEXT("TargetLocation"), _manager->GetMovePoint(index));
}

EBTNodeResult::Type UBTTask_GetMovePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* aIOwner = OwnerComp.GetAIOwner();
	APawn* pawn = aIOwner->GetPawn();

	UMovePointManager* manager = pawn->GetComponentByClass<UMovePointManager>();

	if (manager == nullptr) { return EBTNodeResult::Failed; }

	SetBlackBoardValue(manager, MyBlackboard);

	auto points = manager->GetMovePoints();

	//次の目的地をセット
	SetNextPath(manager, points.Num());

	return EBTNodeResult::Type();
}