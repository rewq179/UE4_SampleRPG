// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolLocation.h"
#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolLocation::UBTTask_FindPatrolLocation()
{
	NodeName = TEXT("FindPatrolLocation");
}

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); // 4.20���ʹ� ~~~V1���� �ٲ�
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMonsterAI::HomeLocation); 
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol)) // origin��ġ���� ��500�� ������ ���� ����Ʈ�� ã�ڴ�.
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAI::PatrolLocation, NextPatrol.Location);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


