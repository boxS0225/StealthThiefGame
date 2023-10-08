// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"
#include "WidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STEALTHTHIEFGAME_API IWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void SetAmmoWidget(const int _remain, const int _hold);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void SetDisplay(const FText& _weaponName, UTexture2D* _texture);
};
