// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterPattern.h"
#include "Monster/Monster.h"

#include "Pattern/Pattern.h"
#include "Manager/GameManager.h"
#include "Player/MainPlayer.h"
#include "Player/PlayerStatus.h"



// Sets default values
AMonsterPattern::AMonsterPattern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanApplyPatternDamage = true;
}

void AMonsterPattern::AddPattern(int32 PatternID)
{
	auto PatternData = GameManager->PatternManager->GetPatternData(PatternID);
	
	Patterns.Add(GetGatherPattern(PatternID, PatternData.Count));
}

FGatherPattern AMonsterPattern::GetGatherPattern(int32 PatternID, int32 Count)
{
	FGatherPattern GatherPattern;
	TArray<APattern*> Patterns;

	for (int32 Index = 0; Index < Count; Index++)
	{
		auto Pattern = GameManager->PatternManager->CreatePatternActor(PatternID);
		Pattern->Monster = Monster;
		Pattern->InitCollision();

		Patterns.Add(Pattern);
	}

	GatherPattern.Patterns = Patterns;

	return GatherPattern;
}

void AMonsterPattern::PatternAnimStart()
{
	SelectPattern.Patterns[0]->PlayUseParticle();
}

void AMonsterPattern::PatternNotifyField() // 0번은 타게팅, 1번 이상부턴 랜덤 Location
{
	SelectPattern.Patterns[0]->PlayNotifyParticle(true);

	for (int32 Index = 1; Index < SelectPattern.Patterns.Num(); Index++)
	{
		SelectPattern.Patterns[Index]->PlayNotifyParticle(false);
	}
}

void AMonsterPattern::PatternDamageField() // 0번은 타게팅, 1번 이상부턴 랜덤 Location
{
	SelectPattern.Patterns[0]->PlayDamageParticle(true);

	for (int32 Index = 1; Index < SelectPattern.Patterns.Num(); Index++)
	{
		SelectPattern.Patterns[Index]->PlayDamageParticle(false);
	}

}

void AMonsterPattern::PatternApplyBuff()
{
	
}

void AMonsterPattern::PatternAnimEnd()
{
	if (SelectPattern.Patterns[0]->PatternData.PatternClass == EPatternClass::EPT_Teleport) // 땅속에 들어가는 경우가 있어서 일단 땜방
	{
		FVector CurLocation = Monster->GetActorLocation();
		CurLocation.Z = SelectPattern.Patterns[0]->TargetLocation.Z + 160.f;

		Monster->SetActorLocation(CurLocation);
	}

	Monster->SetHandType(EHandType::EHT_None);
	
	for (int32 Index = 0; Index < SelectPattern.Patterns.Num(); Index++)
	{
		SelectPattern.Patterns[Index]->SetActiveCollision(false);
	}

	OnPatternEnd.Broadcast();

	bCanApplyPatternDamage = true;
}



void AMonsterPattern::ApplyPatternDamageToTarget(APattern* Pattern)
{
	if (CombatTarget && Pattern && bCanApplyPatternDamage)
	{
		bCanApplyPatternDamage = false;

		switch (Pattern->PatternData.StatusType)
		{
		case EStatusType::EST_CurHp:
			CombatManager->ApplyDamageHP(CombatTarget->PlayerStatus,Pattern->PatternData.CurHP, Monster, Pattern->PatternData.AttackType, true, false);
			break;

		case EStatusType::EST_PerHP:
			CombatManager->ApplyDamageHP(CombatTarget->PlayerStatus, Pattern->PatternData.PerHP, Monster, Pattern->PatternData.AttackType, true, true);
			break;

		case EStatusType::EST_CurST:
			CombatManager->ApplyDamageST(CombatTarget->PlayerStatus, Pattern->PatternData.CurHP, Monster);

			break;

		case EStatusType::EST_PerST:
			CombatManager->ApplyDamageST(CombatTarget->PlayerStatus, Pattern->PatternData.PerHP, Monster);
			break;

		default:
			break;
		}
	}
}

