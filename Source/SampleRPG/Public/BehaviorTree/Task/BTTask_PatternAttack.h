// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatternAttack.generated.h"

/**
 * Pawn의 PatternAttack 애니메이션을 재생/데미지 적용/상태이상 적용을 통합적으로 처리한다.
 */
UCLASS()
class SAMPLERPG_API UBTTask_PatternAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PatternAttack();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(Category = "Value", EditAnywhere)
	int32 AttackNumber; // Montage에 저장되어있는 Attack_?에서 ?에 들어갈 숫자를 의미한다.
	
	bool IsAttacking; // 공격 처리가 진행중이면 True, 끝나면 False
};
