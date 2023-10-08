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
#include "AmmoStruct.h"
#include "WeaponStruct.h"
#include "Animation/AnimMontage.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Blueprint/UserWidget.h"
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

	//武器替えアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> WeaponChangeAction;

	//ジャンプアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> SprintAction;

	//エイムアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> AimAction;

	//発砲アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> FireAction;

	//発砲アクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> ReloadAction;

	//ポインター
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> PointerWidgetClass;

	TObjectPtr<UUserWidget> currentPointerWidget;

	//残弾
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> AmmoWidgetClass;

	TObjectPtr<UUserWidget> currentAmmoWidget;

	//弾
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FAmmoStruct> WeaponAmmoStruct;

	//エイムオフセット
	FVector aimVec;
	FRotator aimRot;

	//敵に影響を与える情報リスト
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

	//持っている武器
	TArray<TObjectPtr<USkeletalMeshComponent>> weaponMeshs;

	TObjectPtr<USkeletalMeshComponent> equipWeapon;

	int equipWeaponNum = 0;
	int equipWeaponCounter = 0;

	//武器を持っているか
	bool hasWeapon;

	//リロード中か
	bool isReload = false;

	//構えているか
	bool isAim = false;

	//発砲できるか
	bool canFire = true;

	//連射用タイマー
	FTimerHandle fireHandle;

	//自身のアニメーション
	TObjectPtr<UAnimInstance> myAnimInstance;

	//装備品データ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UDataTable> WeaponTable;

	FWeaponStruct* weaponInfo;

	//チーム分け用変数
	struct FGenericTeamId TeamId;

	//チーム分け用関数をインターフェースからオーバーライド
	virtual FGenericTeamId GetGenericTeamId() const override;

	//武器を装備
	void EquipWeapon(const bool _hasWeapon, const FName _socketNamem, const bool _hasPistol);

	//ウィジェットをインスタンスにする
	void CreateWidgetInstance();

	FWeaponStruct* SarchWeapon(TArray<TObjectPtr<USkeletalMeshComponent>> _meshs, int _num);

	//発砲処理
	void FireProcess();

	//発砲アニメーション
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

	//継承先で実装(練習)BPで実装する用の指定子なのでcppに関数を書くとエラーになる
	UFUNCTION(BlueprintImplementableEvent)
		void Sprint_Pressed(const FInputActionValue& _value);

	UFUNCTION(BlueprintImplementableEvent)
		void Sprint_Released(const FInputActionValue& _value);


	//エイム用アクション
	void Aiming_Pressed(const FInputActionValue& _value);
	void Aiming_Releassed(const FInputActionValue& _value);

	//発砲アクション
	void Fire_Start(const FInputActionValue& _value);
	void Fire_End(const FInputActionValue& _value);

	UFUNCTION(BlueprintImplementableEvent)
		void ReLoad(const FInputActionValue& _value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	//エイムしているかの判断
	FORCEINLINE void SetIsAim(const bool _isAim) { isAim = _isAim; }

	FORCEINLINE bool SetMyAnimInstance(const TObjectPtr<UAnimInstance> _anim) { myAnimInstance = _anim; return myAnimInstance != nullptr; }

	FORCEINLINE TObjectPtr<UAIPerceptionStimuliSourceComponent> GetStimuliSourceComponent() const { return StimuliSourceComponent; }
	FORCEINLINE bool SetStimuliSourceComponent(const TObjectPtr<UAIPerceptionStimuliSourceComponent> _source) { StimuliSourceComponent = _source; return StimuliSourceComponent != nullptr; }

	FORCEINLINE TObjectPtr<UDataTable> GetWeaponTable() const { return WeaponTable; }

	FORCEINLINE TSubclassOf<UUserWidget> GetWidgetPointerClass() const { return PointerWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetWidgetAmmoClass() const { return AmmoWidgetClass; }
	//////////////////////////////////////////////インターフェースの実装
	//武器を背負う
	virtual void AttachWeapon_Implementation(const FName _attachSocketName, USkeletalMeshComponent* _mesh) override;

	//発砲可能かの設定
	void FireCondition_Implementation(const bool _canFire);

	UFUNCTION(BlueprintCallable)
	FName GetWeaponName() const { return equipWeapon->ComponentTags[0]; }

	UFUNCTION(BlueprintCallable)
	void GetNumberOfAmmo(FName _weaponName, int& _remainAmmo, int& _holdAmmo) const;

	UFUNCTION(BlueprintCallable)
	void SetNuberOfAmmo(FName _weaponName, int _remain, int _hold);

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetAmmoWidget() const {	return currentAmmoWidget; }

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
};

