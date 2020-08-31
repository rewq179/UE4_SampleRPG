// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SkillAttack.h"

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Monster/MonsterPattern.h"

#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SkillAttack::UBTTask_SkillAttack()
{
	bNotifyTick = true; // Æ½ È°¼ºÈ­

	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_SkillAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAI::Target));
	if (nullptr == Monster || nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	Monster->AttackTarget(Target, EPatternClass::EPC_Skill, AttackNumber);
	IsAttacking = true;
	Monster->MonsterPattern->OnPatternEnd.AddLambda([this]() -> void { IsAttacking = false; });

	return EBTNodeResult::InProgress;
}

void UBTTask_SkillAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}





