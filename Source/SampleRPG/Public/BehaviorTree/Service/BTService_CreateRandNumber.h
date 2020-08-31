// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CreateRandNumber.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API UBTService_CreateRandNumber : public UBTService
{
	GENERATED_BODY()
	
	
public:
	UBTService_CreateRandNumber();

	UPROPERTY(Category = "Value", EditAnywhere)
	TArray<int32> Percents;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	int32 GetPatternNumber(int InputNumber);
};
