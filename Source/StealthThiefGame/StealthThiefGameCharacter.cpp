// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthThiefGameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
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
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//カメラオフセット
	aimVec = FVector(0.f, 100.f, 100.f);
	aimRot = FRotator(0.f, 30.f, 40.f);

	weaponInfo = nullptr;

	//メッシュとアニメーションの設定
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	GetMesh()->SetSkeletalMesh(mesh);

	//メッシュの位置と回転の調整
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	//AIparceptionの設定
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	//敵の感覚に影響させる登録
	StimuliSourceComponent->bAutoRegister = true;
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSourceComponent->RegisterWithPerceptionSystem();

	//チームIDの設定
	TeamId = FGenericTeamId(0);

	//マッピングなどは後から変更するかもしれないからここでは設定しないメッシュは練習で設定しておく
}



void AStealthThiefGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//アニメーションインスタンスのゲット
	myAnimInstance = GetMesh()->GetAnimInstance();

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
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStealthThiefGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
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
	}

}

void AStealthThiefGameCharacter::Move(const FInputActionValue& _value)
{
	// input is a Vector2D
	FVector2D MovementVector = _value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AStealthThiefGameCharacter::Look(const FInputActionValue& _value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = _value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AStealthThiefGameCharacter::Fire_Start(const FInputActionValue& _value)
{
	//タイマーのスタート(バインドする関数と、何秒区切りかと、ループするか)
	GetWorldTimerManager().SetTimer(fireHandle, this, &AStealthThiefGameCharacter::FireProcess, 0.1f, true);

}

void AStealthThiefGameCharacter::Fire_End(const FInputActionValue& _value)
{
	FTimerManager& timerManager = GetWorldTimerManager();

	// Handleに登録されたTimerの解放
	timerManager.ClearTimer(fireHandle);

	// このActorが所有するタイマーの解放
	timerManager.ClearAllTimersForObject(this);

}

//ホイールで切り替え
void AStealthThiefGameCharacter::WeaponChange(const FInputActionValue& _value)
{
	//スクロールを獲得
	float upDown = _value.Get<float>();

	//番号の更新
	equipWeaponNum = equipWeaponCounter;

	if (!hasWeapon)//武器を持っていない
	{
		//持っている武器リストから取得
		equipWeapon = weaponMeshs[equipWeaponNum];
		FName weapon = equipWeapon->ComponentTags[0];
		weaponInfo = WeaponTable->FindRow<FWeaponStruct>(weapon, "");
		if (weaponInfo == nullptr) { return; }


		//手に持つ
		EquipWeapon(true, weaponInfo->EquipWeaponSocketName, weaponInfo->HasPistol);

		//武器チェンジ音を鳴らす
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponInfo->EquipSound, GetActorLocation());

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
		equipWeapon = weaponMeshs[num];
		FName weapon = equipWeapon->ComponentTags[0];
		FWeaponStruct* item = WeaponTable->FindRow<FWeaponStruct>(weapon, "");
		if (item == nullptr) { return; }

		//武器を外す
		weaponInfo = nullptr;

		//手から外して固定位置へ
		EquipWeapon(false, item->WeaponSocketName, item->HasPistol);

	}
}

void AStealthThiefGameCharacter::Aiming_Pressed(const FInputActionValue& _value)
{
	SetIsAim(true);

	if (myAnimInstance->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_AimingState(myAnimInstance, true);
	}
	//カメラアームを寄せる
	CameraBoom->TargetArmLength = 150.f;

	//キャラを回転させない
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	//カメラを寄せる
	CameraBoom->SetRelativeLocationAndRotation(aimVec, aimRot, false, nullptr, ETeleportType::None);
}

void AStealthThiefGameCharacter::Aiming_Releassed(const FInputActionValue& _value)
{
	SetIsAim(false);

	if (myAnimInstance->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_AimingState(myAnimInstance, false);
	}
	//カメラを離す
	CameraBoom->TargetArmLength = 300.f;

	//キャラを回転させる
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	//カメラを離す
	CameraBoom->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator, false, nullptr, ETeleportType::None);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Interface


void AStealthThiefGameCharacter::AttachWeapon_Implementation(const FName _attachSocketName, USkeletalMeshComponent* _mesh)
{
	weaponMeshs.Emplace(_mesh);
	_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, _attachSocketName);
}

void AStealthThiefGameCharacter::FireCondition_Implementation(const bool _canFire)
{
	SetCanFire(_canFire);
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

	PlayAnimMontage(weaponInfo->FireMontage);

	GetWorld()->SpawnActor<AActor>(weaponInfo->AmmoClass, equipWeapon->GetSocketTransform(weaponInfo->DropAmmoSocketName));
}

//武器を装備
void AStealthThiefGameCharacter::EquipWeapon(const bool _hasWeapon, const FName _socketName, const bool _hasPistol)
{
	equipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, _socketName);
	hasWeapon = _hasWeapon;

	if (myAnimInstance->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_EquipState(myAnimInstance, hasWeapon, _hasPistol);
	}
}

void AStealthThiefGameCharacter::FireProcess()
{
	//地面にいない場合終了
	if (!GetCharacterMovement()->IsMovingOnGround()) { return; }

	//武器を持っていない場合終了
	if (!hasWeapon) { return; }

	//エイムしていない場合終了
	if (!isAim) { return; }

	//クールタイムなどで発砲できない場合終了
	if (!canFire) { return; }

	//武器がない場合終了
	if (weaponInfo == nullptr) { return; }

	FireAnim();
}