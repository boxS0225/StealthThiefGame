// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "WeaponStruct.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponInterface.h"
#include "StealthThiefGameCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponentを作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	// SceneComponentをRootComponentに設定する
	RootComponent = DefaultSceneRoot;

	//メッシュを作成
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	//ルートの下に配置
	WeaponMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//DataTableから構造体取得
	FName weaponName = WeaponMesh->ComponentTags[0];
	FWeaponStruct* item = WeaponTable->FindRow<FWeaponStruct>(weaponName, "");
	if (item == nullptr) { return; }

	//インターフェースで取得した構造体からプレイヤーに武器を適用
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
	if (playerCharacter->Implements<UWeaponInterface>())
	{
		IWeaponInterface::Execute_AttachWeapon(playerCharacter,item->WeaponSocketName, WeaponMesh);
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

