// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAI.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "ConstructorHelpers.h"

const FName AMonsterAI::HomeLocation(TEXT("HomeLocation"));
const FName AMonsterAI::PatrolLocation(TEXT("PatrolLocation"));
const FName AMonsterAI::Target(TEXT("Target"));
const FName AMonsterAI::InDistance(TEXT("InDistance"));

AMonsterAI::AMonsterAI()
{
	
}

void AMonsterAI::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomeLocation, InPawn->GetActorLocation());
		FVector Location = Blackboard->GetValueAsVector(AMonsterAI::HomeLocation);

		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AI Controller couldn't run!!!!"));
		}
	}
}

void AMonsterAI::StartAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StartTree(*BTAsset, EBTExecutionMode::Looped);
	}
}

void AMonsterAI::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
