// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInNumberRange.h"

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"

#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInNumberRange::UBTDecorator_IsInNumberRange()
{
	NodeName = TEXT("In Number Range");
}

bool UBTDecorator_IsInNumberRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAI::Target));

	if (nullptr == Monster || Target)
	{
		return false;
	}

	int32 PatternNumber = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AMonsterAI::AttackPattern);


	bResult = (Low <= PatternNumber && PatternNumber < High);
	return bResult;
}



