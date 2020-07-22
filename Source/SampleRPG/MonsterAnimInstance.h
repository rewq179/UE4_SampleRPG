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
	float MoveSpeed; // 이동속도에 따라 애니메이션을 따로 재생(블랜드)함.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn; // 애니메이션에 사용할 폰(BP에서 MainController_BP를 등록할 예정)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMonster* Monster;

	
	
};
