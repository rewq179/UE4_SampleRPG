// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAI.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "ConstructorHelpers.h"

const FName AMonsterAI::HomeLocation(TEXT("HomeLocation"));
const FName AMonsterAI::PatrolLocation(TEXT("PatrolLocation"));
const FName AMonsterAI::Target(TEXT("Target"));

AMonsterAI::AMonsterAI()
{
	
}

void AMonsterAI::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("START"));

	if (UseBlackboard(BBAsset, Blackboard))
	{
		UE_LOG(LogTemp, Log, TEXT("Exist"));

		Blackboard->SetValueAsVector(HomeLocation, InPawn->GetActorLocation());
		FVector Location = Blackboard->GetValueAsVector(AMonsterAI::HomeLocation);

		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AI Controller couldn't run!!!!"));
		}
	}
}