// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckDistance.h"

#include "Player/MainPlayer.h"
#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTService_CheckDistance::UBTService_CheckDistance()
{
	NodeName = TEXT("Check Distance");
	Interval = 1.0f;
}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAI::Target));
	if (nullptr == Target)
	{
		return;
	}

	bool bResult = (Target->GetDistanceTo(ControllingPawn) < Distance);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMonsterAI::InDistance, bResult);
}



