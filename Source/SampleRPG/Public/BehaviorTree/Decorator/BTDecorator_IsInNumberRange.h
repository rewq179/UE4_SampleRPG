// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInNumberRange.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API UBTDecorator_IsInNumberRange : public UBTDecorator
{
	GENERATED_BODY()
	
	
public:
	UBTDecorator_IsInNumberRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
private:
	UPROPERTY(Category = "Value", EditAnywhere)
	int32 Low;

	UPROPERTY(Category = "Value", EditAnywhere)
	int32 High;

};
