// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimMontage.h"
#include "AmmoBase.h"
#include "WeaponStruct.generated.h"

/**
 * ����\����
 */
USTRUCT(BlueprintType)
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_BODY()

	//�����ʒu
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName WeaponSocketName;

	//�����ʒu
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName EquipWeaponSocketName;

	//������
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> EquipSound;

	//�s�X�g����
	UPROPERTY(EditAnywhere, Category = If)
	bool HasPistol;

	//���C��
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> FireSound;

	//�}�Y���t���b�V��
	UPROPERTY(EditAnywhere, Category = FX)
	TObjectPtr<UParticleSystem> FireFlash;

	//�e���ʒu
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName MuzzleSocketName;

	//���C���̃A�j���[�V����
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> FireMontage;

	//��䰃N���X
	UPROPERTY(EditAnywhere, Category = Class)
	TObjectPtr<UClass> AmmoClass;

	//�򎺈ʒu
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName DropAmmoSocketName;

public:
	FWeaponStruct();
};
