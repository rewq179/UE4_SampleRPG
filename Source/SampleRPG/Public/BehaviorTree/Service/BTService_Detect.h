// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 몬스터가 자신의 Detect Rance 이내의 타겟(플레이어)을 찾는다.
 * 플레이어가 죽은 상태면 Target이 되지않는다.
 */

UCLASS()
class SAMPLERPG_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
