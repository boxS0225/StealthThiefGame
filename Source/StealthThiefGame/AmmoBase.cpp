// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBase.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AAmmoBase::AAmmoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent���쐬����
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	// SceneComponent��RootComponent�ɐݒ肷��
	RootComponent = DefaultSceneRoot;

	//���b�V�����쐬
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//��������������悤�ɂ���
	AmmoMesh->SetSimulatePhysics(true);
	AmmoMesh->SetMassOverrideInKg(NAME_None, 0.01f, true);

	//���[�g�̉��ɔz�u
	AmmoMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAmmoBase::BeginPlay()
{
	Super::BeginPlay();
	
	TObjectPtr<ACharacter> player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);

	//�v���C���[�̉E����(��)����łĂ���
	FVector rightVec = Cast<AActor>(player)->GetActorRightVector();

	float randVol = FMath::RandRange(0.1f, 0.5f);

	rightVec *= randVol;

	AmmoMesh->AddImpulse(rightVec);

	//�^�C�}�[�̃X�^�[�g(�o�C���h����֐��ƁA���b��؂肩�ƁA���[�v���邩)
	GetWorldTimerManager().SetTimer(fireHandle, this, &AAmmoBase::DeleteAmmo, 10.f, false);
}

void AAmmoBase::DeleteAmmo()
{
	Destroy();
}
