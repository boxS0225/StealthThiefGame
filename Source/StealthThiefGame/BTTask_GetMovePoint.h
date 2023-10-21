// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MovePointManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_GetMovePoint.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTHIEFGAME_API UBTTask_GetMovePoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	void SetNextPath(UMovePointManager* , int _length);
	void SetBlackBoardValue(UMovePointManager* _manager, UBlackboardComponent* _blackboard);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
