// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthThiefGameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StealthThiefGameGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"


//////////////////////////////////////////////////////////////////////////
// AStealthThiefGameCharacter

AStealthThiefGameCharacter::AStealthThiefGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	auto charaMove = GetCharacterMovement();
	charaMove->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	charaMove->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	charaMove->JumpZVelocity = 350.f;
	charaMove->AirControl = 0.35f;
	charaMove->MaxWalkSpeed = 300.f;
	charaMove->MinAnalogWalkSpeed = 20.f;
	charaMove->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	auto cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SetCameraBoom(cameraBoom);
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	cameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	auto camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	SetFollowCamera(camera);
	camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//カメラオフセット
	aimVec = FVector(0.f, 100.f, 100.f);
	aimRot = FRotator(0.f, 30.f, 40.f);

	currentHealth = MaxHealth;

	weaponInfo = nullptr;

	FAmmoStruct ammo;
	ammo.HoldAmmo = 100;
	ammo.RemainAmmo = 0;
	WeaponAmmoStruct.Add("Rifle", ammo);

	ammo.HoldAmmo = 20;
	WeaponAmmoStruct.Add("ShotGun", ammo);

	ammo.HoldAmmo = 30;
	WeaponAmmoStruct.Add("Pistol", ammo);

	//メッシュとアニメーションの設定
	auto mesh = GetMesh();

	USkeletalMesh* skMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	mesh->SetSkeletalMesh(skMesh);

	DissolveMesh = CreateDefaultSubobject<UDissolveMesh>(TEXT("DissolveMesh"));

	//メッシュの位置と回転の調整
	mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	//AIparceptionの設定
	auto aiSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	//敵の感覚に影響させる登録
	aiSource->bAutoRegister = true;

	//セット
	SetStimuliSourceComponent(aiSource);

	//チームIDの設定
	TeamId = FGenericTeamId(AStealthThiefGameGameMode::PlayerTeam);

	//マッピングなどは後から変更するかもしれないからここでは設定しないメッシュは練習で設定しておく
}

//マップに生成されてから
void AStealthThiefGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto source = GetStimuliSourceComponent();

	source->RegisterForSense(UAISense_Sight::StaticClass());
	source->RegisterWithPerceptionSystem();
}

void AStealthThiefGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSourceComponent->RegisterWithPerceptionSystem();

	//アニメーションインスタンスのゲット
	SetMyAnimInstance(GetMesh()->GetAnimInstance());

	//ウィジェットインスタンスの生成
	CreateWidgetInstance();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AStealthThiefGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isZoom)
	{
		currentCameraLerp += 0.05f;
	}
	else
	{
		currentCameraLerp -= 0.05f;
	}
	
	currentCameraLerp = FMath::Clamp(currentCameraLerp, 0.0f, 1.0f);

	auto vec = FMath::Lerp(FVector::ZeroVector, aimVec, currentCameraLerp);
	auto rot = FMath::Lerp(FRotator::ZeroRotator, aimRot, currentCameraLerp);

	//カメラオフセットの移動
	GetCameraBoom()->TargetArmLength = FMath::Lerp(300.f, 150.f, currentCameraLerp);
	GetCameraBoom()->SetRelativeLocationAndRotation(vec, rot, false, nullptr, ETeleportType::None);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStealthThiefGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::Look);
		
		//ChangeWeapon
		EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::WeaponChange);

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::Sprint_Pressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AStealthThiefGameCharacter::Sprint_Released);

		//Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::Aiming_Pressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AStealthThiefGameCharacter::Aiming_Releassed);

		//Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::Fire_Start);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AStealthThiefGameCharacter::Fire_End);

		//Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AStealthThiefGameCharacter::ReLoad);

	}

}

void AStealthThiefGameCharacter::Move(const FInputActionValue& _value)
{
	if (isDeath) { return; }

	// input is a Vector2D
	FVector2D MovementVector = _value.Get<FVector2D>();

	TObjectPtr<AController> controller = GetController();

	AStealthThiefGameGameMode::CheckPointerContent<AController>(controller);

	// find out which way is forward
	const FRotator Rotation = controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void AStealthThiefGameCharacter::Jump()
{
	Super::Jump();

	//アニメーション停止
	if (GetIsReload())
	{
		StopAnimMontage(GetReloadAnim());
		SetIsReload(false);
	}
}

void AStealthThiefGameCharacter::Look(const FInputActionValue& _value)
{
	if (isDeath) { return; }

	// input is a Vector2D
	FVector2D LookAxisVector = _value.Get<FVector2D>();

	AStealthThiefGameGameMode::CheckPointerContent<AController>(GetController());

	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AStealthThiefGameCharacter::Fire_Start(const FInputActionValue& _value)
{
	if (isDeath) { return; }

	//タイマーのスタート(バインドする関数と、何秒区切りかと、ループするか)
	GetWorldTimerManager().SetTimer(fireHandle, this, &AStealthThiefGameCharacter::FireProcess, 0.1f, true);

}

void AStealthThiefGameCharacter::Fire_End(const FInputActionValue& _value)
{
	if (isDeath) { return; }

	FTimerManager& timerManager = GetWorldTimerManager();

	// Handleに登録されたTimerの解放
	timerManager.ClearTimer(fireHandle);

	// このActorが所有するタイマーの解放
	timerManager.ClearAllTimersForObject(this);

}

//ホイールで切り替え
void AStealthThiefGameCharacter::WeaponChange(const FInputActionValue& _value)
{
	if (isDeath) { return; }

	if (weaponMeshs.Num() <= 0) { return; }

	//スクロールを獲得
	float upDown = _value.Get<float>();
	
	//番号の更新
	equipWeaponNum = equipWeaponCounter;

	if (!hasWeapon)//武器を持っていない
	{
		//持っている武器リストから取得
		weaponInfo = SarchWeapon(weaponMeshs, equipWeaponNum);

		//手に持つ
		EquipWeapon(true, weaponInfo->EquipWeaponSocketName, weaponInfo->HasPistol);

		//武器チェンジ音を鳴らす
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponInfo->EquipSound, GetActorLocation());

		AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentAmmoWidget);
		currentAmmoWidget->AddToViewport();

		if (currentAmmoWidget->Implements<UWidgetInterface>())
		{
			int remain, hold;
			GetNumberOfAmmo(GetWeaponName(), remain, hold);

			IWidgetInterface::Execute_SetAmmoWidget(currentAmmoWidget, remain, hold);

			IWidgetInterface::Execute_SetDisplay(currentAmmoWidget, weaponInfo->WeaponName, weaponInfo->WeaponTexture);
		}

		//次の武器へ移動
		equipWeaponCounter++;
		//equipWeaponCounter += upDown <= 0 ? -1 : 1;

		//配列外に行ったら戻す
		if (equipWeaponCounter < 0 || equipWeaponCounter >= weaponMeshs.Num())
		{
			if (equipWeaponCounter < 0)
			{
				equipWeaponCounter = weaponMeshs.Num() - 1;
			}
			else
			{
				equipWeaponCounter = 0;
			}
		}
	}
	else//武器を持っている
	{
		//現在の武器を取得
		int num = equipWeaponNum - 1;
		//int num = equipWeaponNum + upDown <= 0 ? -1 : 1;

		//配列外に行ったら戻す
		if (num < 0 || num >= weaponMeshs.Num())
		{
			if (num < 0)
			{
				num = weaponMeshs.Num() - 1;
			}
			else
			{
				num = 0;
			}
		}

		//持っている武器リストから取得
		FWeaponStruct* item = SarchWeapon(weaponMeshs, num);

		//アニメーション停止
		if (GetIsReload())
		{
			StopAnimMontage(GetReloadAnim());
			SetIsReload(false);
		}

		//武器を外す
		weaponInfo = nullptr;

		//手から外して固定位置へ
		EquipWeapon(false, item->WeaponSocketName, item->HasPistol);

		AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentAmmoWidget);
		currentAmmoWidget->RemoveFromParent();

	}
}

void AStealthThiefGameCharacter::Aiming_Pressed(const FInputActionValue& _value)
{
	if (isDeath) { return; }

	SetIsAim(true);

	isZoom = true;

	if (GetMyAnimInstance()->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_AimingState(myAnimInstance, true);
	}
	//カメラアームを寄せる
	auto cameraboom = GetCameraBoom();

	//キャラを回転させない
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	//ウィジェットの追加
	AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentPointerWidget);
	currentPointerWidget->AddToViewport();
}

void AStealthThiefGameCharacter::Aiming_Releassed(const FInputActionValue& _value)
{
	if (isDeath) { return; }

	SetIsAim(false);

	isZoom = false;

	if (GetMyAnimInstance()->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_AimingState(myAnimInstance, false);
	}
	//カメラを離す
	auto cameraboom = GetCameraBoom();

	//キャラを回転させる
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	//ウィジェットの削除
	AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentPointerWidget);
	currentPointerWidget->RemoveFromParent();
}

float AStealthThiefGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (isDeath) { return 0.f; }

	currentHealth -= DamageAmount;
	currentHealth = currentHealth <= 0.0f ? 0.0f : currentHealth;

	if (currentHealth <= 0.0f) { DieProcess(); };

	float percent = currentHealth / MaxHealth;

	//ウィジェットの追加
	AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentHpWidget);
	currentHpWidget->AddToViewport();

	if (currentHpWidget->Implements<UWidgetInterface>())
	{
		IWidgetInterface::Execute_SetPercent(currentHpWidget, percent);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AStealthThiefGameCharacter::DieProcess()
{
	isDeath = true;

	SetIsAim(false);

	if (GetMyAnimInstance()->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_AimingState(myAnimInstance, false);
	}
	//カメラを離す
	auto cameraboom = GetCameraBoom();
	cameraboom->TargetArmLength = 300.f;

	//キャラを回転させる
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	//カメラを離す
	cameraboom->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator, false, nullptr, ETeleportType::None);

	//ウィジェットの削除
	AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentPointerWidget);
	currentPointerWidget->RemoveFromParent();

	AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentAmmoWidget);
	currentAmmoWidget->RemoveFromParent();

	if (GetMyAnimInstance()->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_DeathCondition(GetMyAnimInstance(), isDeath);
	}

	ReStartPlayer();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Interface


void AStealthThiefGameCharacter::AttachWeapon_Implementation(const FName _attachSocketName, USkeletalMeshComponent* _mesh)
{
	//持っている武器のリストを追加
	weaponMeshs.Emplace(_mesh);
	//背負う
	_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, _attachSocketName);
}

void AStealthThiefGameCharacter::FireCondition_Implementation(const bool _canFire)
{
	SetCanFire(_canFire);
}

void AStealthThiefGameCharacter::CloseWidget_Implementation()
{
	//ウィジェットの削除
	AStealthThiefGameGameMode::CheckPointerContent<UUserWidget>(currentHpWidget);
	currentHpWidget->RemoveFromParent();
}

//チームIDを返す
//Implementation書いてないけどoverrideしてる実装処理
FGenericTeamId AStealthThiefGameCharacter::GetGenericTeamId() const
{
	return TeamId;
}


//////////////////////////////////////////////////////////////////////////

void AStealthThiefGameCharacter::FireAnim()
{
	//音を鳴らす
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponInfo->FireSound, GetActorLocation(), 1.f, 1.f, 0.f);
	UGameplayStatics::PlaySound2D(GetWorld(), weaponInfo->FireSound);

	//マズルフラッシュを生成
	UGameplayStatics::SpawnEmitterAttached(weaponInfo->FireFlash, equipWeapon, weaponInfo->MuzzleSocketName);

	//弾を減らす

	if (currentAmmoWidget->Implements<UWidgetInterface>())
	{
		int remain, hold;
		GetNumberOfAmmo(GetWeaponName(), remain, hold);
		remain--;
		remain = remain < 0 ? 0 : remain;

		SetNuberOfAmmo(GetWeaponName(), remain, hold);

		IWidgetInterface::Execute_SetAmmoWidget(currentAmmoWidget, remain, hold);
	}

	//反動アニメーション再生
	PlayAnimMontage(weaponInfo->FireMontage);

	//薬莢生成
	GetWorld()->SpawnActor<AActor>(weaponInfo->AmmoClass, equipWeapon->GetSocketTransform(weaponInfo->DropAmmoSocketName));

	//中心に弾が出るかの確認
	FVector boomVec = GetCameraBoom()->GetComponentLocation();
	FVector fwdVec = GetCameraBoom()->GetForwardVector();
	fwdVec *= 10000.f;
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, boomVec, boomVec + fwdVec, ECollisionChannel::ECC_Visibility);
	
	AStealthThiefGameGameMode::CheckPointerContent<AActor>(hit.GetActor());

	UGameplayStatics::ApplyDamage(hit.GetActor(), 10.f, GetWorld()->GetFirstPlayerController(), nullptr, nullptr);
}

//武器を装備
void AStealthThiefGameCharacter::EquipWeapon(const bool _hasWeapon, const FName _socketName, const bool _hasPistol)
{
	equipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, _socketName);
	hasWeapon = _hasWeapon;

	if (GetMyAnimInstance()->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_EquipState(GetMyAnimInstance(), hasWeapon, _hasPistol);
	}
}

bool AStealthThiefGameCharacter::ReloadCheck()
{
	//地面にいない場合終了
	if (!GetCharacterMovement()->IsMovingOnGround()) { return true; }

	//武器を持っていない場合終了
	if (!hasWeapon) { return true; }

	//エイムしていない場合終了
	if (!isAim) { return true; }

	//クールタイムなどで発砲できない場合終了
	if (!canFire) { return true; }

	//素手の場合終了
	if (weaponInfo == nullptr) { return true; }

	//弾がない場合終了
	int remain, hold;
	GetNumberOfAmmo(GetWeaponName(), remain, hold);
	if (remain <= 0) { return true; }

	//リロード中の場合終了
	if (GetIsReload()) { return true; }

	return false;
}

void AStealthThiefGameCharacter::FireProcess()
{
	if (ReloadCheck()) { return; }

	FireAnim();
}

//ウィジェットをインスタンスにする
void AStealthThiefGameCharacter::CreateWidgetInstance()
{
	//ポインターのクラスをセット
	TSubclassOf<UUserWidget> pointerWidget = GetWidgetPointerClass();

	AStealthThiefGameGameMode::CheckPointerContent<UClass>(pointerWidget);
	currentPointerWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), pointerWidget);

	TSubclassOf<UUserWidget> ammoWidget = GetWidgetAmmoClass();

	AStealthThiefGameGameMode::CheckPointerContent<UClass>(ammoWidget);
	currentAmmoWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), ammoWidget);

	TSubclassOf<UUserWidget> hpWidget = GetWidgetHpClass();
	AStealthThiefGameGameMode::CheckPointerContent<UClass>(hpWidget);
	currentHpWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), hpWidget);
}

FWeaponStruct* AStealthThiefGameCharacter::SarchWeapon(TArray<TObjectPtr<USkeletalMeshComponent>> _meshs, int _num)
{
	equipWeapon = _meshs[_num];
 	FName weapon = equipWeapon->ComponentTags[0];
	FWeaponStruct* item = GetWeaponTable()->FindRow<FWeaponStruct>(weapon, "");
	AStealthThiefGameGameMode::CheckPointerContent<FWeaponStruct>(item);

	return item;
}

void AStealthThiefGameCharacter::GetNumberOfAmmo(FName _weaponName, int& _remainAmmo, int& _holdAmmo) const
{
	auto ammo = WeaponAmmoStruct.Find(_weaponName);
		
	_remainAmmo = ammo->RemainAmmo;
	_holdAmmo = ammo->HoldAmmo;
}

void AStealthThiefGameCharacter::SetNuberOfAmmo(FName _weaponName, int _remain, int _hold)
{
	FAmmoStruct ammo;

	ammo.RemainAmmo = _remain;
	ammo.HoldAmmo = _hold;

	WeaponAmmoStruct.Add(_weaponName, ammo);
}

UAnimMontage* AStealthThiefGameCharacter::GetReloadAnim() const
{
	FName weapon = equipWeapon->ComponentTags[0];
	FWeaponStruct* item = GetWeaponTable()->FindRow<FWeaponStruct>(weapon, "");
	AStealthThiefGameGameMode::CheckPointerContent<FWeaponStruct>(item);

	return item->ReloadMontage;
}
