// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "WeaponInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISense_Sight.h"
#include "Components/TimelineComponent.h"
#include "WeaponDataTable.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "StealthThiefGameCharacter.generated.h"


UCLASS(config=Game)
class AStealthThiefGameCharacter : public ACharacter, public IGenericTeamAgentInterface, public IWeaponInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	//武器替えアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> WeaponChangeAction;

	//ジャンプアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	//敵に影響を与える情報リスト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

	//持っている武器
	TArray<USkeletalMeshComponent*> weaponMeshs;

	int equipWeaponNum = 0;
	int equipWeaponCounter = 0;

	//武器を持っているか
	bool hasWeapon;

	//自身のアニメーション
	TObjectPtr<UAnimInstance> myAnimInstance;

	//装備品データ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponTable;

	TObjectPtr<USkeletalMeshComponent> equipWeapon;

	//チーム分け用変数
	struct FGenericTeamId TeamId;

	//チーム分け用関数をインターフェースからオーバーライド
	virtual FGenericTeamId GetGenericTeamId() const override;

	//武器を装備
	void EquipWeapon(const bool _hasWeapon,const FName _socketNamem, const bool _hasPistol);

public:
	AStealthThiefGameCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& _value);

	/** Called for looking input */
	void Look(const FInputActionValue& _value);

	void WeaponChange(const FInputActionValue& _value);

	//継承先で実装(練習)BPで実装する用の指定子なのでcppに関数を書くとエラーになる
	UFUNCTION(BlueprintImplementableEvent)
	void Sprint_Pressed(const FInputActionValue& _value);

	UFUNCTION(BlueprintImplementableEvent)
	void Sprint_Released(const FInputActionValue& _value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE TObjectPtr<UCameraComponent> GetFollowCamera() const { return FollowCamera; }

	//武器を背負う
	virtual void AttachWeapon_Implementation(const FName _attachSocketName, USkeletalMeshComponent* _mesh) override;
};

