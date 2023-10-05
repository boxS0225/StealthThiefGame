// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePointManager.h"
#include "StealthThiefGameGameMode.h"

// Sets default values for this component's properties
UMovePointManager::UMovePointManager()
{
	
}


// Called when the game starts
void UMovePointManager::BeginPlay()
{
	Super::BeginPlay();

	//配列外参照チェック
	AStealthThiefGameGameMode::CheckArraySize(GetMovePoints());
}