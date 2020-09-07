// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanChargingAttack.generated.h"

/**
 * 차징 공격에는 쿨타임(15초 이상)이 존재한다. 쿨타임시에는 불가능함. 
 */

UCLASS()
class SAMPLERPG_API UBTDecorator_CanChargingAttack : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CanChargingAttack();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
	
	
};
