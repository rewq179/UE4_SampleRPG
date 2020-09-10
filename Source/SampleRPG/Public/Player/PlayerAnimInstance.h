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
	float MoveSpeed; // 이동속도에 따라 애니메이션을 따로 재생(블랜드)함.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir; // 점프 체크

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn; // 애니메이션에 사용할 폰(BP에서 MainController_BP를 등록할 예정)

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateProperty();



};
