// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StealthThiefGameGameMode.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyAIControllerBase.h"
#include "WeaponStruct.h"
#include "MovePointManager.h"
#include "EnemyInterface.h"
#include "EnemyBase.generated.h"

UCLASS()
class AEnemyBase : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

	//ïêäÌÉÅÉbÉVÉÖ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMovePointManager> MovePoints;

	const FName weaponName = TEXT("Rifle");

	//HP
	const float MaxHealth = 100.f;
	float currentHealth;

	bool isDeath = false;

	//ïêäÌëïíÖ
	void EquipWeapon(FName _weapon);

	void DieProcess();

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Attack_Implementation() override;

	virtual float GetHp_Implementation() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE bool SetWeaponMesh(const TObjectPtr<USkeletalMeshComponent> _mesh) { WeaponMesh = _mesh; return WeaponMesh != nullptr; }

	FORCEINLINE TObjectPtr<UDataTable> GetWeaponTable() const { return WeaponTable; }

	UFUNCTION(BlueprintCallable)
	float GetcurrentHealth() const { return currentHealth; }
};
