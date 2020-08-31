// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SkillAttack.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API UBTTask_SkillAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SkillAttack();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(Category = "Value", EditAnywhere)
	int32 AttackNumber;
	bool IsAttacking;
	
	
};
