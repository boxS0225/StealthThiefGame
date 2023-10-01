// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WeaponInterface.h"
#include "WeaponStruct.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISense_Sight.h"
#include "AnimInterface.h"
#include "Components/TimelineComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "StealthThiefGameCharacter.generated.h"


UCLASS(config=Game)
class AStealthThiefGameCharacter : public ACharacter, public IGenericTeamAgentInterface, public IWeaponInterface, public IAnimInterface
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

	//����ւ��A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> WeaponChangeAction;

	//�W�����v�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	//�G�C���A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AimAction;

	//���C�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireAction;

	//�G�C���I�t�Z�b�g
	FVector aimVec;
	FRotator aimRot;

	//�G�ɉe����^�����񃊃X�g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

	//�����Ă��镐��
	TArray<TObjectPtr<USkeletalMeshComponent>> weaponMeshs;

	TObjectPtr<USkeletalMeshComponent> equipWeapon;

	int equipWeaponNum = 0;
	int equipWeaponCounter = 0;

	//����������Ă��邩
	bool hasWeapon;

	//�\���Ă��邩
	bool isAim = false;

	//���C�ł��邩
	bool canFire = true;

	//�A�˗p�^�C�}�[
	FTimerHandle fireHandle;

	//���g�̃A�j���[�V����
	TObjectPtr<UAnimInstance> myAnimInstance;

	//�����i�f�[�^
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponTable;

	FWeaponStruct* weaponInfo;

	//�`�[�������p�ϐ�
	struct FGenericTeamId TeamId;

	//�`�[�������p�֐����C���^�[�t�F�[�X����I�[�o�[���C�h
	virtual FGenericTeamId GetGenericTeamId() const override;

	//����𑕔�
	void EquipWeapon(const bool _hasWeapon,const FName _socketNamem, const bool _hasPistol);

	//���C����
	void FireProcess();

	//���C�A�j���[�V����
	void FireAnim();

public:
	AStealthThiefGameCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& _value);

	/** Called for looking input */
	void Look(const FInputActionValue& _value);

	void WeaponChange(const FInputActionValue& _value);

	//�p����Ŏ���(���K)BP�Ŏ�������p�̎w��q�Ȃ̂�cpp�Ɋ֐��������ƃG���[�ɂȂ�
	UFUNCTION(BlueprintImplementableEvent)
	void Sprint_Pressed(const FInputActionValue& _value);

	UFUNCTION(BlueprintImplementableEvent)
	void Sprint_Released(const FInputActionValue& _value);


	//�G�C���p�A�N�V����
	void Aiming_Pressed(const FInputActionValue& _value);
	void Aiming_Releassed(const FInputActionValue& _value);

	//���C�A�N�V����
	void Fire_Start(const FInputActionValue& _value);
	void Fire_End(const FInputActionValue& _value);

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
	//�G�C�����Ă��邩�̔��f
	FORCEINLINE void SetIsAim(bool _isAim) { isAim = _isAim; }

	FORCEINLINE void SetCanFire(bool _canFire) { canFire = _canFire; }

	//////////////////////////////////////////////�C���^�[�t�F�[�X�̎���
	//�����w����
	virtual void AttachWeapon_Implementation(const FName _attachSocketName, USkeletalMeshComponent* _mesh) override;

	//���C�\���̐ݒ�
	void FireCondition_Implementation(const bool _canFire);
};

