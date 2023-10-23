// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyInterface.h"
#include "AIController.h"
#include "BTService_Hp.h"

void UBTService_Hp::SetBlackBoardValue(UBlackboardComponent* _blackboard, float _hp)
{
	_blackboard->SetValueAsFloat(GetSelectedBlackboardKey(), _hp);
}

void UBTService_Hp::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* aIOwner = OwnerComp.GetAIOwner();
	APawn* pawn = aIOwner->GetPawn();

	if (pawn->Implements<UEnemyInterface>())
	{
		//キャラごとの攻撃をする
		float hp = IEnemyInterface::Execute_GetHp(pawn);

		//ブラックボードにデータをセット
		SetBlackBoardValue(myBlackboard, hp);
	}
}
