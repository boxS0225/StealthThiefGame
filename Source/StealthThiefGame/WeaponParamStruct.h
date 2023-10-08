// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponParamStruct.generated.h"

USTRUCT(BlueprintType)
struct FWeaponParamStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	FName WeaponTag;

public:	
	// Sets default values for this actor's properties
	FWeaponParamStruct();
};
