// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthThiefGameGameMode.h"
#include "StealthThiefGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStealthThiefGameGameMode::AStealthThiefGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/StealthThiefGame/Player/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
