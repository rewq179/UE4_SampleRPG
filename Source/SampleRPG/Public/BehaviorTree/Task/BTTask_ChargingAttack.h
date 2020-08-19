// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChargingAttack.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API UBTTask_ChargingAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ChargingAttack();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsCharging;
	
	float OrginWalkSpeed;
};
