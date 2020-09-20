// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/CombatManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataTableManager.h"
#include "Manager/DungeonManager.h"
#include "Manager/ItemManager.h"
#include "Manager/SkillManager.h"

#include "Monster/Monster.h"

#include "Player/MainPlayer.h"
#include "Player/Inventory.h"
#include "Player/PlayerStatus.h"
#include "Player/PlayerCombat.h"

#include "Kismet/GameplayStatics.h"

#include "TimerManager.h"

// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeTime = 0.f;
}

void ACombatManager::ApplyDamageHP(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, EAttackType AttackType, bool bIsPlayerDamaged, bool bIsPercent)
{
	if (DamagedActor && (BaseDamage != 0.f))
	{
		if (bIsPlayerDamaged)
		{
			auto PlayerStatus = Cast<APlayerStatus>(DamagedActor);

			if (bIsPercent)
			{
				BaseDamage = GetPercentBaseDamage(BaseDamage, PlayerStatus->Stat.MaxHP);
			}

			if (PlayerStatus)
			{
				PlayerStatus->TakeDamageHP(BaseDamage, DamageCauser, AttackType);
			}
		}

		else
		{
			auto Monster = Cast<AMonster>(DamagedActor);

			if (bIsPercent)
			{
				BaseDamage = GetPercentBaseDamage(BaseDamage, Monster->Status.MaxHP);
			}

			if (Monster)
			{
				Monster->TakeDamageHP(BaseDamage, DamageCauser, AttackType);
			}
		}
	}
}

void ACombatManager::AdjustDamageHP(AActor* DamagedActor, float BaseDamage, bool bIsPercent)
{
	if (bIsPercent)
	{
		BaseDamage = GetPercentBaseDamage(BaseDamage, MainPlayer->PlayerStatus->Stat.MaxHP);
	}

	MainPlayer->PlayerStatus->AdjustHP(BaseDamage, false);
}

void ACombatManager::ApplyDamageST(AActor* DamagedActor, float BaseDamage, bool bIsPercent)
{
	if (DamagedActor && (BaseDamage != 0.f))
	{
		auto PlayerStatus = Cast<APlayerStatus>(DamagedActor);

		if (bIsPercent)
		{
			BaseDamage = GetPercentBaseDamage(BaseDamage, PlayerStatus->Stat.MaxStamina);
		}

		if (PlayerStatus)
		{
			PlayerStatus->TakeDamageST(BaseDamage);
		}
	}
}

float ACombatManager::GetPercentBaseDamage(float Percent, float MaxValue)
{
	return MaxValue * 0.01f * Percent;
}

// 몬스터 보상 //

void ACombatManager::PlayerDeath()
{
	GameManager->DungeonManager->SetReviveData();
}

bool ACombatManager::CanPlayerRevive()
{
	if (GameManager->DungeonManager->ReviveCount > 0)
	{
		GameManager->DungeonManager->ReviveCount--;

		return true;
	}

	return false;
}


void ACombatManager::MonsterDeath(AMonster* Monster)
{
	MainPlayer->PlayerStatus->AddExp(Monster->Status.Exp);
	MainPlayer->Inventory->AddGold(Monster->Status.Gold);
	RemoveWidgetMonster(Monster);

	GameManager->ItemManager->GetMonsterItem(Monster->Status.ByProductID, Monster->Status.RewardID, Monster->GetActorLocation());
	GameManager->DungeonManager->CheckTrigger(Monster->Status.ID);
}

// 버프 적용 //

void ACombatManager::SetBuffToTarget(AActor* Target, int32 SkillID, bool bIsPlayer)
{
	if (bIsPlayer)
	{
		auto MainPlayer = Cast<AMainPlayer>(Target);

	}

	else
	{

	}
}


// 상태이상 적용 //

void ACombatManager::SetDebuffToPlayer(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::EAT_Poison:
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACombatManager::SetPoison, 1.f, true, 0.f);
		break;

	case EAttackType::EAT_Frostbite:
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACombatManager::SetFrostbite, 1.f, true, 0.f);
		break;
	}
}

void ACombatManager::SetPoison()
{
	auto SkillData = SkillManager->GetSkillData(0);

	if (IsLifeTimeOver(SkillData.DurationTime))
	{
		return;
	}

	if (!MainPlayer->PlayerStatus->SkillMaps.Find(SkillData.SkillID))
	{
		MainPlayer->PlayerStatus->SkillMaps.Add(SkillData.SkillID, SkillData);
	}

	AdjustDamageHP(MainPlayer->PlayerStatus, SkillManager->GetSkillDamage(SkillData.SkillID), true);
}

void ACombatManager::SetFrostbite()
{
	auto SkillData = SkillManager->GetSkillData(1);

	if (IsLifeTimeOver(SkillData.DurationTime))
	{
		return;
	}

	if (!MainPlayer->PlayerStatus->SkillMaps.Find(SkillData.SkillID))
	{
		MainPlayer->PlayerStatus->SkillMaps.Add(SkillData.SkillID, SkillData);
	}

	ApplyDamageST(MainPlayer->PlayerStatus, SkillManager->GetSkillDamage(SkillData.SkillID), true);
}

bool ACombatManager::IsLifeTimeOver(float DurationTime)
{
	LifeTime += GetWorldTimerManager().GetTimerElapsed(TimerHandle);
	
	if (LifeTime > DurationTime)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		LifeTime = 0.f;

		return true;
	}

	return false;
}

// HUD //

void ACombatManager::AddTargetMonster(class AMonster* Monster)
{
	MainPlayer->PlayerCombat->AddTargetMonster(Monster);
}
void ACombatManager::AddWidgetMonster(class AMonster* Monster)
{
	MainPlayer->PlayerCombat->AddWidgetMonster(Monster);
}
void ACombatManager::RemoveWidgetMonster(class AMonster* Monster)
{
	MainPlayer->PlayerCombat->RemoveWidgetMonster(Monster);
}