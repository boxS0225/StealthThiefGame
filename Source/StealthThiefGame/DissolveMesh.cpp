// Fill out your copyright notice in the Description page of Project Settings.


#include "DissolveMesh.h"
#include "StealthThiefGameGameMode.h"

// Sets default values for this component's properties
UDissolveMesh::UDissolveMesh()
{

}

UMaterialInstanceDynamic* UDissolveMesh::CreateDissolveMesh(UPrimitiveComponent* _mesh, int _elementIndex)
{
	auto skMesh = Cast<USkeletalMeshComponent>(_mesh);

	//スケルタルメッシュをセット
	if (skMesh != nullptr)
	{
		auto material = AddDissolveMesh(skMesh->CreateDynamicMaterialInstance(_elementIndex));
		return material;
	}

	auto stMesh = Cast<UStaticMeshComponent>(_mesh);
	//スタティックメッシュをセット
	if (skMesh != nullptr)
	{
		auto material = AddDissolveMesh(stMesh->CreateDynamicMaterialInstance(_elementIndex));
		return material;
	}

	return nullptr;
}

void UDissolveMesh::SetMaterialParameter(UMaterialInstanceDynamic* _target, FName _paramName, float _value)
{
	auto mid = GetDissolveMesh(_target);
	//Nullチェック
	AStealthThiefGameGameMode::CheckPointerContent<UMaterialInstanceDynamic>(mid);
	//パラメータセット
	mid->SetScalarParameterValue(_paramName, _value);
}