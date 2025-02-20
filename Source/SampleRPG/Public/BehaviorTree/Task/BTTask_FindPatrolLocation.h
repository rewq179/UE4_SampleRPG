// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolLocation.generated.h"

/**
 * Pawn의 Patrol 지점을 지정한다.
 */

UCLASS()
class SAMPLERPG_API UBTTask_FindPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public: 
	UBTTask_FindPatrolLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
