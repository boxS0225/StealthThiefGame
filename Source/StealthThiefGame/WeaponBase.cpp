// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "WeaponStruct.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponInterface.h"
#include "StealthThiefGameCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent���쐬����
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	// SceneComponent��RootComponent�ɐݒ肷��
	RootComponent = DefaultSceneRoot;

	//���b�V�����쐬
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	//���[�g�̉��ɔz�u
	WeaponMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//DataTable����\���̎擾
	FName weaponName = WeaponMesh->ComponentTags[0];
	FWeaponStruct* item = WeaponTable->FindRow<FWeaponStruct>(weaponName, "");
	if (item == nullptr) { return; }

	//�C���^�[�t�F�[�X�Ŏ擾�����\���̂���v���C���[�ɕ����K�p
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
	if (playerCharacter->Implements<UWeaponInterface>())
	{
		IWeaponInterface::Execute_AttachWeapon(playerCharacter,item->WeaponSocketName, WeaponMesh);
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

