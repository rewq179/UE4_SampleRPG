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

// Called when the game starts or when spawned
void AMonsterPattern::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMonsterPattern::AddPattern(int32 PatternID)
{
	auto PatternData = GameManager->PatternManager->GetPatternData(PatternID);
	
	Patterns.Add(SetPatternStruct(PatternID, PatternData.Count));
}

FGatherPatterns AMonsterPattern::SetPatternStruct(int32 PatternID, int32 Count)
{
	FGatherPatterns PatternStruct;
	TArray<APattern*> Patterns;

	for (int32 Index = 0; Index < Count; Index++)
	{
		auto Pattern = GameManager->PatternManager->CreatePatternActor(PatternID);
		Pattern->Monster = Monster;

		Pattern->SetCollisionSize();
		Pattern->SetActiveCollision(false);

		Patterns.Add(Pattern);
	}

	PatternStruct.Patterns = Patterns;

	return PatternStruct;
}

void AMonsterPattern::PatternAnimStart()
{
	SelectedPatterns.Patterns[0]->PlayUseParticle();
}

void AMonsterPattern::PatternNotifyField()
{
		SelectedPatterns.Patterns[0]->PlayNotifyParticle(true);

	for (int32 Index = 1; Index < SelectedPatterns.Patterns.Num(); Index++)
	{
		SelectedPatterns.Patterns[Index]->PlayNotifyParticle(false);
	}
}

void AMonsterPattern::PatternDamageField()
{
	SelectedPatterns.Patterns[0]->PlayDamageParticle(true);

	for (int32 Index = 1; Index < SelectedPatterns.Patterns.Num(); Index++)
	{
		SelectedPatterns.Patterns[Index]->PlayDamageParticle(false);
	}

}

void AMonsterPattern::PatternApplyBuff()
{

}

void AMonsterPattern::PatternAnimEnd()
{
	if (SelectedPatterns.Patterns[0]->PatternData.PatternClass == EPatternClass::EPT_Teleport)
	{
		FVector CurLocation = Monster->GetActorLocation();
		CurLocation.Z = SelectedPatterns.Patterns[0]->TargetLocation.Z + 160.f;

		Monster->SetActorLocation(CurLocation);
		//Monster->SetCapsuleComponent(true);
	}

	Monster->SetHandType(EHandType::EHT_None);
	
	for (int32 Index = 0; Index < SelectedPatterns.Patterns.Num(); Index++)
	{
		SelectedPatterns.Patterns[Index]->Target = nullptr;
		SelectedPatterns.Patterns[Index]->SetActiveCollision(false);
	}

	OnPatternEnd.Broadcast();

	bCanApplyPatternDamage = true;
}



void AMonsterPattern::ApplyPatternDamageToTarget(APattern* SelectedPattern)
{
	if (CombatTarget && SelectedPattern && bCanApplyPatternDamage)
	{
		bCanApplyPatternDamage = false;

		switch (SelectedPattern->PatternData.DamagedType)
		{
		case EDamagedType::EDT_CurHp:
			CombatManager->ApplyDamageHP(CombatTarget->PlayerStatus, SelectedPattern->PatternData.CurHP, Monster, SelectedPattern->PatternData.AttackType, true, false);
			break;

		case EDamagedType::EDT_PerHP:
			CombatManager->ApplyDamageHP(CombatTarget->PlayerStatus, SelectedPattern->PatternData.PerHP, Monster, SelectedPattern->PatternData.AttackType, true, true);
			break;

		case EDamagedType::EDT_CurST:
			CombatManager->ApplyDamageST(CombatTarget->PlayerStatus, SelectedPattern->PatternData.CurHP, Monster, SelectedPattern->PatternData.AttackType, false);

			break;

		case EDamagedType::EDT_PerST:
			CombatManager->ApplyDamageST(CombatTarget->PlayerStatus, SelectedPattern->PatternData.PerHP, Monster, SelectedPattern->PatternData.AttackType, true);
			break;

		default:
			break;
		}
	}
}

