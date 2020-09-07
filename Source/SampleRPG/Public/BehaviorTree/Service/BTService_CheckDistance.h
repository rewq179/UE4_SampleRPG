// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDistance.generated.h"

/**
 * (조종되고있는 Pawn과 Target의 거리값 <= Distance Value) 이면 True, 아니면 False
 */

UCLASS()
class SAMPLERPG_API UBTService_CheckDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckDistance();
	
	UPROPERTY(Category = "Value", EditAnywhere)
	float Distance;

protected:
	virtual void TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;


};
