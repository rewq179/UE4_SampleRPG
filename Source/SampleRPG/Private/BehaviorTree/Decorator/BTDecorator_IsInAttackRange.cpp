// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange.h"
#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"

#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("In Attack Range");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
	{
		return false;
	}
	
	auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAI::Target));
	if (nullptr == Target)
	{
		return false;
	}

	bResult = (Target->GetDistanceTo(Monster) <= Monster->Status.AttackRange);
	return bResult;
}


