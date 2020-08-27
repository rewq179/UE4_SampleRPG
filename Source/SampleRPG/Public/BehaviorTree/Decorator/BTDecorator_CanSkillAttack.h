// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanSkillAttack.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API UBTDecorator_CanSkillAttack : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CanSkillAttack();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


	
	
};
