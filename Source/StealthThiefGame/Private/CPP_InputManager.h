// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_InputManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCPP_InputManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_InputManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetUpInput(UInputComponent* InputComponent);

    void  btpRTrigger();
    void  btpRPadLeft();
    void  btpRPadTop();
    void  btpRPadBottom();
    void  btpRPadRight();
    void  btpRThoulder();
    void  btpRGrip();

    void  btpLTrigger();
    void  btpLPadLeft();
    void  btpLPadTop();
    void  btpLPadBottom();
    void  btpLPadRight();
    void  btpLThoulder();
    void  btpLGrip();

    void  btrRTrigger();
    void  btrRPadLeft();
    void  btrRPadTop();
    void  btrRPadBottom();
    void  btrRPadRight();
    void  btrRThoulder();
    void  btrRGrip();

    void  btrLTrigger();
    void  btrLPadLeft();
    void  btrLPadTop();
    void  btrLPadBottom();
    void  btrLPadRight();
    void  btrLThoulder();
    void  btrLGrip();

    void r_PadX(float axis);
    void r_PadY(float axis);

    void l_PadX(float axis);
    void l_PadY(float axis);

    //各ボタンの現在状態を保持します。
    UPROPERTY(BlueprintReadOnly)
        bool r_Trigger = false;
    UPROPERTY(BlueprintReadOnly)
        bool r_PadLeft = false;
    UPROPERTY(BlueprintReadOnly)
        bool r_PadTop = false;
    UPROPERTY(BlueprintReadOnly)
        bool r_PadBottom = false;
    UPROPERTY(BlueprintReadOnly)
        bool r_PadRight = false;
    UPROPERTY(BlueprintReadOnly)
        bool r_Thoulder = false;
    UPROPERTY(BlueprintReadOnly)
        bool r_Grip = false;

    UPROPERTY(BlueprintReadOnly)
        bool l_Trigger = false;
    UPROPERTY(BlueprintReadOnly)
        bool l_PadLeft = false;
    UPROPERTY(BlueprintReadOnly)
        bool l_PadTop = false;
    UPROPERTY(BlueprintReadOnly)
        bool l_PadBottom = false;
    UPROPERTY(BlueprintReadOnly)
        bool l_PadRight = false;
    UPROPERTY(BlueprintReadOnly)
        bool l_Thoulder = false;
    UPROPERTY(BlueprintReadOnly)
        bool l_Grip = false;

    UPROPERTY(BlueprintReadOnly)
        FVector2D r_PadInput;
    UPROPERTY(BlueprintReadOnly)
        FVector2D l__PadInput;
};