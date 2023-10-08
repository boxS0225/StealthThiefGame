// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AmmoStruct.generated.h"

USTRUCT(BlueprintType)
struct FAmmoStruct : public FTableRowBase
{
	GENERATED_BODY()

	//éËéùÇøíe
	UPROPERTY(EditAnywhere, Category = Ammo)
	int HoldAmmo;

	//écíe
	UPROPERTY(EditAnywhere, Category = Ammo)
	int RemainAmmo;

public:	
	// Sets default values for this actor's properties
	FAmmoStruct();
};
