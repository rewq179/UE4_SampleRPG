// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NormalAttack.h"
#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_NormalAttack::UBTTask_NormalAttack()
{
	bNotifyTick = true; // Æ½ È°¼ºÈ­

	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAI::Target));

	Monster->AttackTarget(Target, EAttackType::EAT_Normal);
	IsAttacking = true;
	Monster->OnAttackEnd.AddLambda([this]() -> void { IsAttacking = false;});

	return EBTNodeResult::InProgress;
}

void UBTTask_NormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


