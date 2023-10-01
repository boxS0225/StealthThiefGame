// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimMontage.h"
#include "WeaponStruct.generated.h"

/**
 * ïêäÌç\ë¢ëÃ
 */
USTRUCT(BlueprintType)
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = SocketName)
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, Category = SocketName)
	FName EquipWeaponSocketName;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditAnywhere, Category = If)
	bool HasPistol;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditAnywhere, Category = FX)
	TObjectPtr<UParticleSystem> FireFlash;

	UPROPERTY(EditAnywhere, Category = SocketName)
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> FireMontage;

public:
	FWeaponStruct();
};
