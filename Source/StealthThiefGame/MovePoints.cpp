// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePoints.h"

// Sets default values
AMovePoints::AMovePoints()
{
	// SceneComponent‚ğì¬‚·‚é
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	// SceneComponent‚ğRootComponent‚Éİ’è‚·‚é
	SetRootComponent(DefaultSceneRoot);

}
