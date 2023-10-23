// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIControllerBase.h"
#include "StealthThiefGameGameMode.h"

AEnemyAIControllerBase::AEnemyAIControllerBase()
{
	SetGenericTeamId(FGenericTeamId(AStealthThiefGameGameMode::EnemyTeam));

	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");

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
	TObjectPtr<UBehaviorTree> tree = GetMyBehaviorTree();

	//�c���[��NULL�`�F�b�N
	AStealthThiefGameGameMode::CheckPointerContent<UBehaviorTree>(tree);
	//�c���[���N��
	RunBehaviorTree(tree);
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