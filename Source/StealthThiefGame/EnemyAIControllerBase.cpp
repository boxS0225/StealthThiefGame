// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIControllerBase.h"
#include "StealthThiefGameGameMode.h"

AEnemyAIControllerBase::AEnemyAIControllerBase()
{
	SetGenericTeamId(FGenericTeamId(AStealthThiefGameGameMode::EnemyTeam));
	
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");

	///Game/StealthThiefGame/Enemy/Main/BT_MainEnemy
	ConstructorHelpers::FObjectFinder<UBehaviorTree> tree(TEXT("/Game/StealthThiefGame/Enemy/BehindOfPlayer/BT_BehindOfPlayer"));

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	SetMyBehaviorTree(tree.Object);

	//感覚のセット
	AISenseConfigSight = SetSenseSight();
	AISenseConfigHearing = SetSenseHearing();

	//感覚情報を追加
	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfigHearing);
	AIPerceptionComponent->SetDominantSense(AISenseConfigSight->GetSenseImplementation());
	SetPerceptionComponent(*AIPerceptionComponent);
}

void AEnemyAIControllerBase::BeginPlay()
{
	//これないとOnProssess呼ばれない
	Super::BeginPlay();
}

void AEnemyAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BehaviorComp->StartTree(*GetMyBehaviorTree());
	BlackboardComp->InitializeBlackboard(*(GetMyBehaviorTree()->BlackboardAsset));


	//RunBehaviorTree(GetMyBehaviorTree());
	//GetBlackboardComponent()->InitializeBlackboard(*(GetMyBehaviorTree()->BlackboardAsset));
}

void AEnemyAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

TObjectPtr<UAISenseConfig_Sight> AEnemyAIControllerBase::SetSenseSight()
{
	TObjectPtr<UAISenseConfig_Sight> sense = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");;
	sense->PeripheralVisionAngleDegrees = 90.f;
	//敵味方すべて取得
	sense->DetectionByAffiliation.bDetectEnemies = true;
	sense->DetectionByAffiliation.bDetectFriendlies = true;
	sense->DetectionByAffiliation.bDetectNeutrals = true;

	return sense;
}


TObjectPtr<UAISenseConfig_Hearing> AEnemyAIControllerBase::SetSenseHearing()
{
	TObjectPtr<UAISenseConfig_Hearing> sense = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	//敵味方すべて取得
	sense->DetectionByAffiliation.bDetectEnemies = true;
	sense->DetectionByAffiliation.bDetectFriendlies = true;
	sense->DetectionByAffiliation.bDetectNeutrals = true;
	return sense;
}

//自身に対する相手の振る舞い（友好・中立・敵対）
ETeamAttitude::Type AEnemyAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			//ベースの比較を使う場合
			//return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());

			//カスタム
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (GetGenericTeamId() == OtherTeamID) return ETeamAttitude::Friendly;
			else if (OtherTeamID == FGenericTeamId(255)) return ETeamAttitude::Neutral;
			else return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Neutral;
}