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

	//�X�P���^�����b�V�����Z�b�g
	if (skMesh != nullptr)
	{
		auto material = AddDissolveMesh(skMesh->CreateDynamicMaterialInstance(_elementIndex));
		return material;
	}

	auto stMesh = Cast<UStaticMeshComponent>(_mesh);
	//�X�^�e�B�b�N���b�V�����Z�b�g
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
	//Null�`�F�b�N
	AStealthThiefGameGameMode::CheckPointerContent<UMaterialInstanceDynamic>(mid);
	//�p�����[�^�Z�b�g
	mid->SetScalarParameterValue(_paramName, _value);
}