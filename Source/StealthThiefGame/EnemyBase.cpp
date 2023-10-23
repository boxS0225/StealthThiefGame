// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAIControllerBase.h"
#include "BrainComponent.h"
#include "AnimInterface.h"
#include "GenericTeamAgentInterface.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	charaMove->JumpZVelocity = 700.f;
	charaMove->AirControl = 0.35f;
	charaMove->MaxWalkSpeed = 300.f;
	charaMove->MinAnalogWalkSpeed = 20.f;
	charaMove->BrakingDecelerationWalking = 2000.f;

	//���b�V���̐ݒ�
	auto mesh = GetMesh();
	USkeletalMesh* bodyMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	mesh->SetSkeletalMesh(bodyMesh);
	
	//���b�V���̈ʒu�Ɖ�]�̒���
	mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	//���탁�b�V���̐���
	auto weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMwsh"));

	SetWeaponMesh(weaponMesh);
	USkeletalMesh* weaponSkMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A"));
	weaponMesh->SetSkeletalMesh(weaponSkMesh);

 	MovePoints = CreateDefaultSubobject<UMovePointManager>(TEXT("MovePoints"));

	MovePoints->SetupAttachment(mesh);

	//���탁�b�V���̔z�u
	weaponMesh->SetupAttachment(mesh);

	currentHealth = MaxHealth;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(weaponName);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::EquipWeapon(FName _weapon)
{
	auto weaponTable = GetWeaponTable();

	//Null�`�F�b�N
	AStealthThiefGameGameMode::CheckPointerContent<UDataTable>(weaponTable);

	//���C�t���𑕔�
	FWeaponStruct* item = weaponTable->FindRow<FWeaponStruct>(_weapon, "");

	//���݂��邩�`�F�b�N
	AStealthThiefGameGameMode::CheckPointerContent<FWeaponStruct>(item);
	
	//�Z�b�g
	GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, item->EquipWeaponSocketName);
}

void AEnemyBase::Attack_Implementation()
{
	FWeaponStruct* item = GetWeaponTable()->FindRow<FWeaponStruct>(weaponName, "");

	//����炷
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponInfo->FireSound, GetActorLocation(), 1.f, 1.f, 0.f);
	UGameplayStatics::PlaySound2D(GetWorld(), item->FireSound);

	//�}�Y���t���b�V���𐶐�
	UGameplayStatics::SpawnEmitterAttached(item->FireFlash, WeaponMesh, item->MuzzleSocketName);

	//�����A�j���[�V�����Đ�
	PlayAnimMontage(item->FireMontage);

	//��䰐���
	GetWorld()->SpawnActor<AActor>(item->AmmoClass, WeaponMesh->GetSocketTransform(item->DropAmmoSocketName));

	//���S�ɒe���o�邩�̊m�F
	FVector startVec = WeaponMesh->GetSocketLocation(item->MuzzleSocketName);
	FVector fwdVec = startVec.ForwardVector;
	fwdVec *= 10000.f;
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, startVec, startVec + fwdVec, ECollisionChannel::ECC_Visibility);

	if (hit.GetActor() != nullptr)
		UGameplayStatics::ApplyDamage(hit.GetActor(), 10.f, GetWorld()->GetFirstPlayerController(), nullptr, nullptr);
}

float AEnemyBase::GetHp_Implementation()
{
	return GetcurrentHealth();
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (isDeath) { return 0.0f; }

	currentHealth -= DamageAmount;
	currentHealth = currentHealth <= 0.0f ? 0.0f : currentHealth;

	if (currentHealth <= 0.0f) { DieProcess(); };

	float percent = currentHealth / MaxHealth;

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyBase::DieProcess()
{
	isDeath = true;

	//�r�w�C�r�A�c���[�̒�~
	auto controller = Cast<AAIController>(AIControllerClass);
	auto brain = controller->BrainComponent;
	brain->StopLogic(TEXT("NPC Dying"));

	if (GetMesh()->GetAnimInstance()->Implements<UAnimInterface>())
	{
		IAnimInterface::Execute_DeathCondition(GetMesh()->GetAnimInstance(), isDeath);
	}
}
