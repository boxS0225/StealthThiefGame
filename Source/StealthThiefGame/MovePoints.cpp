// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePoints.h"

// Sets default values
AMovePoints::AMovePoints()
{
	// SceneComponentを作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	// SceneComponentをRootComponentに設定する
	SetRootComponent(DefaultSceneRoot);

}
