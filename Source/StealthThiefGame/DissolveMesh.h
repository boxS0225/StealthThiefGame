// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DissolveMesh.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHTHIEFGAME_API UDissolveMesh : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = mesh)
	TArray<UMaterialInstanceDynamic*> dissolveMeshs;

public:	
	// Sets default values for this component's properties
	UDissolveMesh();

protected:

public:	
	//�}�e���A���C���X�^���X�̍쐬
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* CreateDissolveMesh(UPrimitiveComponent* _mesh, int _elementIndex);

	//�}�e���A���C���X�^���X�̐��l��ύX
	UFUNCTION(BlueprintCallable)
	void SetMaterialParameter(UMaterialInstanceDynamic* _target, FName _paramName, float _value);

	FORCEINLINE TObjectPtr<UMaterialInstanceDynamic> AddDissolveMesh(const TObjectPtr<UMaterialInstanceDynamic> _mid) { dissolveMeshs.Emplace(_mid); return _mid; }
	FORCEINLINE TObjectPtr<UMaterialInstanceDynamic> GetDissolveMesh(TObjectPtr<UMaterialInstanceDynamic> _target) const { int num = dissolveMeshs.Find(_target); return dissolveMeshs[num]; }
};
