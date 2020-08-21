// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_CanChargingAttack.h"

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanChargingAttack::UBTDecorator_CanChargingAttack()
{
	NodeName = TEXT("Can Charging Attack");
}

bool UBTDecorator_CanChargingAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
	{
		return false;
	}

	bResult = Monster->bCanChargingAttack;
	return bResult;
}





