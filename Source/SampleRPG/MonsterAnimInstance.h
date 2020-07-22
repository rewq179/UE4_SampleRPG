// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateProperty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MoveSpeed; // �̵��ӵ��� ���� �ִϸ��̼��� ���� ���(����)��.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn; // �ִϸ��̼ǿ� ����� ��(BP���� MainController_BP�� ����� ����)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMonster* Monster;

	
	
};
