// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBase.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AAmmoBase::AAmmoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponentを作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	// SceneComponentをRootComponentに設定する
	RootComponent = DefaultSceneRoot;

	//メッシュを作成
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//物理挙動をするようにする
	AmmoMesh->SetSimulatePhysics(true);
	AmmoMesh->SetMassOverrideInKg(NAME_None, 0.01f, true);

	//ルートの下に配置
	AmmoMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAmmoBase::BeginPlay()
{
	Super::BeginPlay();
	
	TObjectPtr<ACharacter> player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);

	//プレイヤーの右方向(薬室)からでてくる
	FVector rightVec = Cast<AActor>(player)->GetActorRightVector();

	float randVol = FMath::RandRange(0.1f, 0.5f);

	rightVec *= randVol;

	AmmoMesh->AddImpulse(rightVec);

	//タイマーのスタート(バインドする関数と、何秒区切りかと、ループするか)
	GetWorldTimerManager().SetTimer(fireHandle, this, &AAmmoBase::DeleteAmmo, 10.f, false);
}

void AAmmoBase::DeleteAmmo()
{
	Destroy();
}
