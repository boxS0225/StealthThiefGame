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
#include "WidgetInterface.h"
#include "AnimInterface.h"
#include "DissolveMesh.h"
#include "AmmoStruct.h"
#include "WeaponStruct.h"
#include "Animation/AnimMontage.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Blueprint/UserWidget.h"
#include "StealthThiefGameCharacter.generated.h"


UCLASS(config=Game)
class AStealthThiefGameCharacter : public ACharacter, public IGenericTeamAgentInterface, public IWeaponInterface, public IAnimInterface, public IWidgetInterface
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

	//���C�A�N�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> ReloadAction;

	//�|�C���^�[
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> PointerWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> currentPointerWidget;

	//�c�e
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> AmmoWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> currentAmmoWidget;

	//HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> hpWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> currentHpWidget;

	//�e
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FAmmoStruct> WeaponAmmoStruct;

	//�n���郁�b�V��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDissolveMesh> DissolveMesh;

	TObjectPtr<UMaterialInstanceDynamic> bodyMesh;
	TObjectPtr<UMaterialInstanceDynamic> logoMesh;

	//�G�C���I�t�Z�b�g
	FVector aimVec;
	FRotator aimRot;

	//�����Ă��镐��
	TArray<TObjectPtr<USkeletalMeshComponent>> weaponMeshs;

	TObjectPtr<USkeletalMeshComponent> equipWeapon;

	int equipWeaponNum = 0;
	int equipWeaponCounter = 0;

	//����������Ă��邩
	bool hasWeapon;

	//HP
	const float MaxHealth = 100.f;
	float currentHealth;

	//�G�C���̃Y�[��
	bool isZoom = false;
	float currentCameraLerp = 0.0f;

	//�����[�h����
	bool isReload = false;

	//�\���Ă��邩
	bool isAim = false;

	//���C�ł��邩
	bool canFire = true;

	bool isDeath = false;

	//�A�˗p�^�C�}�[
	FTimerHandle fireHandle;

	//���g�̃A�j���[�V����
	TObjectPtr<UAnimInstance> myAnimInstance;

	//�����i�f�[�^
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponTable;

	FWeaponStruct* weaponInfo;

protected:

	//�`�[�������p�ϐ�
	struct FGenericTeamId TeamId;

	//�G�ɉe����^�����񃊃X�g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent = nullptr;

	virtual void PostInitializeComponents() override;

	//�`�[�������p�֐����C���^�[�t�F�[�X����I�[�o�[���C�h
	virtual FGenericTeamId GetGenericTeamId() const override;

	//����𑕔�
	void EquipWeapon(const bool _hasWeapon, const FName _socketNamem, const bool _hasPistol);

	//�E�B�W�F�b�g���C���X�^���X�ɂ���
	void CreateWidgetInstance();

	FWeaponStruct* SarchWeapon(TArray<TObjectPtr<USkeletalMeshComponent>> _meshs, int _num);

	//���C����
	void FireProcess();

	//���C�A�j���[�V����
	void FireAnim();

	bool ReloadCheck();

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

	UFUNCTION(BlueprintImplementableEvent)
	void ReLoad(const FInputActionValue& _value);

	UFUNCTION(BlueprintImplementableEvent)
	void ReStartPlayer();

	void DieProcess();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE bool SetCameraBoom(const TObjectPtr<USpringArmComponent> _cameraArm) { CameraBoom = _cameraArm; return CameraBoom != nullptr; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE TObjectPtr<UCameraComponent> GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool SetFollowCamera(const TObjectPtr<UCameraComponent> _camera) { FollowCamera = _camera; return FollowCamera != nullptr; }

	FORCEINLINE void SetCanFire(const bool _canFire) { canFire = _canFire; }

	//�G�C�����Ă��邩�̔��f
	FORCEINLINE void SetIsAim(const bool _isAim) { isAim = _isAim; }

	FORCEINLINE bool SetMyAnimInstance(const TObjectPtr<UAnimInstance> _anim) { myAnimInstance = _anim; return myAnimInstance != nullptr; }

	FORCEINLINE TObjectPtr<UAIPerceptionStimuliSourceComponent> GetStimuliSourceComponent() const { return StimuliSourceComponent; }
	FORCEINLINE bool SetStimuliSourceComponent(const TObjectPtr<UAIPerceptionStimuliSourceComponent> _source) { StimuliSourceComponent = _source; return StimuliSourceComponent != nullptr; }

	FORCEINLINE TObjectPtr<UDataTable> GetWeaponTable() const { return WeaponTable; }

	FORCEINLINE TSubclassOf<UUserWidget> GetWidgetPointerClass() const { return PointerWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetWidgetAmmoClass() const { return AmmoWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetWidgetHpClass() const { return hpWidgetClass; }

	UFUNCTION(BlueprintCallable)
		FName GetWeaponName() const { return equipWeapon->ComponentTags[0]; }

	UFUNCTION(BlueprintCallable)
		void GetNumberOfAmmo(FName _weaponName, int& _remainAmmo, int& _holdAmmo) const;

	UFUNCTION(BlueprintCallable)
		void SetNuberOfAmmo(FName _weaponName, int _remain, int _hold);

	UFUNCTION(BlueprintCallable)
		UUserWidget* GetAmmoWidget() const { return currentAmmoWidget; }

	UFUNCTION(BlueprintCallable)
		UAnimMontage* GetReloadAnim() const;

	UFUNCTION(BlueprintCallable)
		UAnimInstance* GetMyAnimInstance() const { return myAnimInstance; }

	UFUNCTION(BlueprintCallable)
		bool GetHasWeapon() const { return hasWeapon; }

	UFUNCTION(BlueprintCallable)
		bool GetCanFire() const { return canFire; }

	UFUNCTION(BlueprintCallable)
		bool GetIsReload() const { return isReload; }

	UFUNCTION(BlueprintCallable)
		void SetIsReload(const bool _isReload) { isReload = _isReload; }

	UFUNCTION(BlueprintCallable)
	TArray<USkeletalMeshComponent*> GetWeaponMesh() { return weaponMeshs; }

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetLogoMesh() const { return logoMesh; }

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetBodyMesh() const { return bodyMesh; }

	UFUNCTION(BlueprintCallable)
	void SetLogoMesh(UMaterialInstanceDynamic* _mid)  { logoMesh = _mid; }

	UFUNCTION(BlueprintCallable)
	void SetBodyMesh(UMaterialInstanceDynamic* _mid) { bodyMesh = _mid; }

	UFUNCTION(BlueprintCallable)
	UDissolveMesh* GetDissolveMesh() const { return DissolveMesh; }

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetCurrentHpWidget() const { return currentHpWidget; }

	//////////////////////////////////////////////�C���^�[�t�F�[�X�̎���
	//�����w����
	virtual void AttachWeapon_Implementation(const FName _attachSocketName, USkeletalMeshComponent* _mesh) override;

	//���C�\���̐ݒ�
	void FireCondition_Implementation(const bool _canFire);

	virtual void CloseWidget_Implementation() override;
};

