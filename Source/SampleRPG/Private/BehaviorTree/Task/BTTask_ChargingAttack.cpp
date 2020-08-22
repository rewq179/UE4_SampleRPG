// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChargingAttack.h"

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_ChargingAttack::UBTTask_ChargingAttack()
{
	bNotifyTick = true; // Æ½ È°¼ºÈ­

	IsCharging = false;
}

EBTNodeResult::Type UBTTask_ChargingAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAI::Target));

	Monster->AttackTarget(Target, EAttackType::EAT_Charging);
	IsCharging = true;
	Monster->OnChargingEnd.AddLambda([this]() -> void { IsCharging = false; });

	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->Status.FollowSpeed * 1.5f; 

	return EBTNodeResult::InProgress;
}

void UBTTask_ChargingAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsCharging)
	{
		auto Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

		if (nullptr != Monster)
		{
			Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->Status.NormalSpeed;

		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}





