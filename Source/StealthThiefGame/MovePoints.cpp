// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePoints.h"

// Sets default values
AMovePoints::AMovePoints()
{
	// SceneComponent���쐬����
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	// SceneComponent��RootComponent�ɐݒ肷��
	SetRootComponent(DefaultSceneRoot);

}
