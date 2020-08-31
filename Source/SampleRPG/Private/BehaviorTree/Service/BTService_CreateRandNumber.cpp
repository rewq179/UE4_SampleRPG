// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CreateRandNumber.h"

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_CreateRandNumber::UBTService_CreateRandNumber()
{
	NodeName = TEXT("Create Rand Number");

	Interval = 1.0f;
	bNotifyBecomeRelevant = true;
}

void UBTService_CreateRandNumber::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	int Rand = FMath::RandRange(0, 99);

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMonsterAI::AttackPattern, GetPatternNumber(Rand));
}

int32 UBTService_CreateRandNumber::GetPatternNumber(int InputNumber)
{
	int High = 0;
	int Low = 0;

	for (int Index = 0; Index < Percents.Num(); Index++) // 20 30 50
	{
		High += Percents[Index];

		if (Low <= InputNumber && InputNumber <= High)
		{
			UE_LOG(LogTemp, Log, TEXT("%d <= %d < %d // Index : %d"), Low, InputNumber, High, Index);

			return Index;
		}

		Low += Percents[Index];
	}

	return -1;
}





