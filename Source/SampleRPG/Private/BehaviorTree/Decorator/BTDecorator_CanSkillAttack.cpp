// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_CanSkillAttack.h"

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanSkillAttack::UBTDecorator_CanSkillAttack()
{
	NodeName = TEXT("Can Skill Attack");
}

bool UBTDecorator_CanSkillAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
	{
		return false;
	}

	//bResult = Monster->bCanSkillAttack;
	return bResult;
}





