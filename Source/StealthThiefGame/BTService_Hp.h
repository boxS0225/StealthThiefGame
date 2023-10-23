// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_Hp.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTHIEFGAME_API UBTService_Hp : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	void SetBlackBoardValue(UBlackboardComponent* _blackboard, float _hp);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,	float DeltaSeconds) override;
};
