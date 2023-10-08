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
 * 武器構造体
 */
USTRUCT(BlueprintType)
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_BODY()

	//装備位置
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName WeaponSocketName;

	//装着位置
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName EquipWeaponSocketName;

	//装着音
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> EquipSound;

	//ピストルか
	UPROPERTY(EditAnywhere, Category = If)
	bool HasPistol;

	//発砲音
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> FireSound;

	//マズルフラッシュ
	UPROPERTY(EditAnywhere, Category = FX)
	TObjectPtr<UParticleSystem> FireFlash;

	//銃口位置
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName MuzzleSocketName;

	//発砲時のアニメーション
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> FireMontage;

	//リロードのアニメーション
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UAnimMontage> ReloadMontage;

	//薬莢クラス
	UPROPERTY(EditAnywhere, Category = Class)
	TObjectPtr<UClass> AmmoClass;

	//薬室位置
	UPROPERTY(EditAnywhere, Category = SocketName)
	FName DropAmmoSocketName;

	//武器タイプ
	UPROPERTY(EditAnywhere, Category = TypeName)
	FName WeaponType;

	//武器の名前
	UPROPERTY(EditAnywhere, Category = WeaponName)
	FText WeaponName;

	//武器画像
	UPROPERTY(EditAnywhere, Category = Texture)
	TObjectPtr<UTexture2D> WeaponTexture;

public:
	FWeaponStruct();
};
