// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	//���탁�b�V���̔z�u
	weaponMesh->SetupAttachment(mesh);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(TEXT("Rifle"));
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
