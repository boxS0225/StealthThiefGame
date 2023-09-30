// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
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
	FName EquipWeaponSocket;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditAnywhere, Category = If)
	bool HasPistol;

public:
	FWeaponStruct();
};
