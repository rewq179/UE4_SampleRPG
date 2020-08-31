// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/CombatManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataTableManager.h"
#include "Manager/ItemManager.h"

#include "Monster/Monster.h"

#include "Player/MainPlayer.h"
#include "Player/Inventory.h"
#include "Player/PlayerStatus.h"
#include "Player/PlayerCombat.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
	
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

void ACombatManager::ApplyDamageST(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, EAttackType DamagedType, bool bIsPercent)
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
			PlayerStatus->TakeDamageST(BaseDamage, DamageCauser, DamagedType);
		}
	}
}

float ACombatManager::GetPercentBaseDamage(float Percent, float MaxHP)
{
	return MaxHP / Percent;
}


void ACombatManager::MonsterDeath(AMonster* Monster)
{
	MainPlayer->PlayerStatus->AddExp(Monster->Status.Exp);
	MainPlayer->Inventory->AddGold(Monster->Status.Gold);
	RemoveWidgetMonster(Monster);

	GameManager->ItemManager->GetMonsterItem(Monster->Status.ByProductID, Monster->Status.RewardID, Monster->GetActorLocation());
}



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