// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthThiefGameGameMode.generated.h"

UCLASS(minimalapi)
class AStealthThiefGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

	template<typename T>
	static bool IsNullPointer(const T* _val) { return _val != nullptr; }
	static bool IsEmptyArray(const int _length) { return _length > 0; }
	static bool IsOutOfBounds(const int _num, const int _length) { return _num < _length && _num >= 0; }

public:
	AStealthThiefGameGameMode();

	static const int PlayerTeam = 1;
	static const int EnemyTeam = 10;

	//��z�񂩃`�F�b�N
	template<typename T>
	static void CheckArraySize(const TArray<T> _val) { ensureAlwaysMsgf(IsEmptyArray(_val.Num()), TEXT("NullArrayException")); }

	//�z��O�ɍs�������`�F�b�N
	template<typename T>
	static void CheckOutOfBounds(const int _num, const TArray<T> _val) { ensureAlwaysMsgf(IsOutOfBounds(_num, _val.Num()), TEXT("IndexOutOfBoundsException")); }

	//Null�`�F�b�N
	template<typename T>
	static void CheckPointerContent(const T* _val) { ensureAlwaysMsgf(IsNullPointer<T>(_val), TEXT("NullPointerException")); }
};



