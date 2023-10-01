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

	//�J�����I�t�Z�b�g
	aimVec = FVector(0.f, 100.f, 100.f);
	aimRot = FRotator(0.f, 30.f, 40.f);

	weaponInfo = nullptr;

	//���b�V���ƃA�j���[�V�����̐ݒ�
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	GetMesh()->SetSkeletalMesh(mesh);

	//���b�V���̈ʒu�Ɖ�]�̒���
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	//AIparception�̐ݒ�
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	//�G�̊��o�ɉe��������o�^
	StimuliSourceComponent->bAutoRegister = true;
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	StimuliSourceComponent->RegisterWithPerceptionSystem();

	//�`�[��ID�̐ݒ�
	TeamId = FGenericTeamId(0);

	//�}�b�s���O�Ȃǂ͌ォ��ύX���邩������Ȃ����炱���ł͐ݒ肵�Ȃ����b�V���͗��K�Őݒ肵�Ă���
}



void AStealthThiefGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//�A�j���[�V�����C���X�^���X�̃Q�b�g
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
	//�^�C�}�[�̃X�^�[�g(�o�C���h����֐��ƁA���b��؂肩�ƁA���[�v���邩)
	GetWorldTimerManager().SetTimer(fireHandle, this, &AStealthThiefGameCharacter::FireProcess, 0.1f, true);

}

void AStealthThiefGameCharacter::Fire_End(const FInputActionValue& _value)
{
	FTimerManager& timerManager = GetWorldTimerManager();

	// Handle�ɓo�^���ꂽTimer�̉��
	timerManager.ClearTimer(fireHandle);

	// ����Actor�����L����^�C�}�[�̉��
	timerManager.ClearAllTimersForObject(this);

}

//�z�C�[���Ő؂�ւ�
void AStealthThiefGameCharacter::WeaponChange(const FInputActionValue& _value)
{
	//�X�N���[�����l��
	float upDown = _value.Get<float>();

	//�ԍ��̍X�V
	equipWeaponNum = equipWeaponCounter;

	if (!hasWeapon)//����������Ă��Ȃ�
	{
		//�����Ă��镐�탊�X�g����擾
		equipWeapon = weaponMeshs[equipWeaponNum];
		FName weapon = equipWeapon->ComponentTags[0];
		weaponInfo = WeaponTable->FindRow<FWeaponStruct>(weapon, "");
		if (weaponInfo == nullptr) { return; }


		//��Ɏ���
		EquipWeapon(true, weaponInfo->EquipWeaponSocketName, weaponInfo->HasPistol);

		//����`�F���W����炷
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponInfo->EquipSound, GetActorLocation());

		//���̕���ֈړ�
		equipWeaponCounter++;
		//equipWeaponCounter += upDown <= 0 ? -1 : 1;

		//�z��O�ɍs������߂�
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
	else//����������Ă���
	{
		//���݂̕�����擾
		int num = equipWeaponNum - 1;
		//int num = equipWeaponNum + upDown <= 0 ? -1 : 1;

		//�z��O�ɍs������߂�
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

		//�����Ă��镐�탊�X�g����擾
		equipWeapon = weaponMeshs[num];
		FName weapon = equipWeapon->ComponentTags[0];
		FWeaponStruct* item = WeaponTable->FindRow<FWeaponStruct>(weapon, "");
		if (item == nullptr) { return; }

		//������O��
		weaponInfo = nullptr;

		//�肩��O���ČŒ�ʒu��
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
	//�J�����A�[�����񂹂�
	CameraBoom->TargetArmLength = 150.f;

	//�L��������]�����Ȃ�
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	//�J�������񂹂�
	CameraBoom->SetRelativeLocationAndRotation(aimVec, aimRot, false, nullptr, ETeleportType::None);
}

void AStealthThiefGameCharacter::Aiming_Releassed(const FInputActionValue& _value)
{
	SetIsAim(false);

	if (myAnimInstance->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_AimingState(myAnimInstance, false);
	}
	//�J�����𗣂�
	CameraBoom->TargetArmLength = 300.f;

	//�L��������]������
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	//�J�����𗣂�
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

//�`�[��ID��Ԃ�
//Implementation�����ĂȂ�����override���Ă��������
FGenericTeamId AStealthThiefGameCharacter::GetGenericTeamId() const
{
	return TeamId;
}


//////////////////////////////////////////////////////////////////////////

void AStealthThiefGameCharacter::FireAnim()
{
	//����炷
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponInfo->FireSound, GetActorLocation(), 1.f, 1.f, 0.f);
	UGameplayStatics::PlaySound2D(GetWorld(), weaponInfo->FireSound);

	//�}�Y���t���b�V���𐶐�
	UGameplayStatics::SpawnEmitterAttached(weaponInfo->FireFlash, equipWeapon, weaponInfo->MuzzleSocketName);

	PlayAnimMontage(weaponInfo->FireMontage);

	GetWorld()->SpawnActor<AActor>(weaponInfo->AmmoClass, equipWeapon->GetSocketTransform(weaponInfo->DropAmmoSocketName));
}

//����𑕔�
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
	//�n�ʂɂ��Ȃ��ꍇ�I��
	if (!GetCharacterMovement()->IsMovingOnGround()) { return; }

	//����������Ă��Ȃ��ꍇ�I��
	if (!hasWeapon) { return; }

	//�G�C�����Ă��Ȃ��ꍇ�I��
	if (!isAim) { return; }

	//�N�[���^�C���ȂǂŔ��C�ł��Ȃ��ꍇ�I��
	if (!canFire) { return; }

	//���킪�Ȃ��ꍇ�I��
	if (weaponInfo == nullptr) { return; }

	FireAnim();
}