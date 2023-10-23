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

	//���o�̃Z�b�g
	AISenseConfigSight = SetSenseSight();
	AISenseConfigHearing = SetSenseHearing();

	//���o����ǉ�
	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfigHearing);
	AIPerceptionComponent->SetDominantSense(AISenseConfigSight->GetSenseImplementation());
	SetPerceptionComponent(*AIPerceptionComponent);
}

void AEnemyAIControllerBase::BeginPlay()
{
	//����Ȃ���OnProssess�Ă΂�Ȃ�
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
	//�G�������ׂĎ擾
	sense->DetectionByAffiliation.bDetectEnemies = true;
	sense->DetectionByAffiliation.bDetectFriendlies = true;
	sense->DetectionByAffiliation.bDetectNeutrals = true;

	return sense;
}


TObjectPtr<UAISenseConfig_Hearing> AEnemyAIControllerBase::SetSenseHearing()
{
	TObjectPtr<UAISenseConfig_Hearing> sense = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	//�G�������ׂĎ擾
	sense->DetectionByAffiliation.bDetectEnemies = true;
	sense->DetectionByAffiliation.bDetectFriendlies = true;
	sense->DetectionByAffiliation.bDetectNeutrals = true;
	return sense;
}

//���g�ɑ΂��鑊��̐U�镑���i�F�D�E�����E�G�΁j
ETeamAttitude::Type AEnemyAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			//�x�[�X�̔�r���g���ꍇ
			//return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());

			//�J�X�^��
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (GetGenericTeamId() == OtherTeamID) return ETeamAttitude::Friendly;
			else if (OtherTeamID == FGenericTeamId(255)) return ETeamAttitude::Neutral;
			else return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Neutral;
}