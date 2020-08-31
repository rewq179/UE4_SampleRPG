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
	auto Pattern = GameManager->PatternManager->CreatePatternActor(PatternID);
	Pattern->Monster = Monster;

	Pattern->SetCollisionSize();

	PatternMaps.Add(PatternID, Pattern);
}

void AMonsterPattern::PatternAnimStart()
{
	SelectedPattern->PlayParticle(0);
}

void AMonsterPattern::PatternNotifyField()
{
	SelectedPattern->PlayParticle(1);
}

void AMonsterPattern::PatternDamageField()
{
	SelectedPattern->SetActiveCollision(true);
	SelectedPattern->PlayParticle(2);
}

void AMonsterPattern::PatternAnimEnd()
{
	Monster->SetCombatCollisionEnabled(false);
	SelectedPattern->Target = nullptr;
	SelectedPattern->SetActiveCollision(false);

	OnPatternEnd.Broadcast();

	bCanApplyPatternDamage = true;
}



void AMonsterPattern::ApplyPatternDamageToTarget()
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

