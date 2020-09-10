// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class AMainPlayer* MainPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MoveSpeed; // �̵��ӵ��� ���� �ִϸ��̼��� ���� ���(����)��.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir; // ���� üũ

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn; // �ִϸ��̼ǿ� ����� ��(BP���� MainController_BP�� ����� ����)

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateProperty();



};
