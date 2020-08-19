// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Player/MainPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();

	AMonster* Monster = Cast<AMonster>(ControllingPawn);
	float DetectRange = Monster->Status.DetectRange;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRange),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			auto MainPlayer = Cast<AMainPlayer>(OverlapResult.GetActor());

			if (MainPlayer && MainPlayer->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAI::Target, MainPlayer); // ≈∏∞Ÿ ¿˙¿Â
				
				auto Target = Cast<AMainPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAI::Target));

				return;
			}
		}
	}
	
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAI::Target, nullptr);
	}
}


