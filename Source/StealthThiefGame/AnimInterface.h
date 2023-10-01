// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STEALTHTHIEFGAME_API IAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//武器を持つアニメーション
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface")
	void EquipState(const bool _hasWeapon, const bool _hasPistol);

	//武器を持つアニメーション
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface")
	void AimingState(const bool _isAiming);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void FireCondition(const bool _canFire);
};
