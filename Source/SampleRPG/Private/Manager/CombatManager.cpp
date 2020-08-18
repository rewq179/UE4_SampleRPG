// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/CombatManager.h"
#include "Manager/GameManager.h"
#include "Manager/DataTableManager.h"
#include "Manager/ItemManager.h"

#include "Player/MainPlayer.h"
#include "Player/Inventory.h"

#include "Monster/Monster.h"

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


void ACombatManager::MonsterDeath(AMonster* Monster)
{
	MainPlayer->AddExp(Monster->Status.Exp);
	MainPlayer->Inventory->AddGold(Monster->Status.Gold);

	GameManager->ItemManager->GetMonsterItem(Monster->Status.ByProductID, Monster->Status.RewardID, Monster->GetActorLocation());
}
