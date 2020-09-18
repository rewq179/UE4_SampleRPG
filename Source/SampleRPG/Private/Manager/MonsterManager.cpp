// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/MonsterManager.h"
#include "Manager/GameManager.h"

#include "Monster.h"

#include "Engine/World.h"

// Sets default values
AMonsterManager::AMonsterManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FMonsterTable AMonsterManager::GetMonsterData(int32 MonsterID)
{
	AMonster* Monster = MonsterMaps[MonsterID].GetDefaultObject();

	SetMonsterData(Monster, MonsterID);

	return Monster->Status;
}


void AMonsterManager::SetMonsterData(AMonster* Monster, int32 MonsterID)
{
	FMonsterTable* MonsterTableRow = GameManager->DataTableManager->GetMonsterTableData(MonsterID);

	if (MonsterTableRow)
	{
		Monster->Status.Name = (*MonsterTableRow).Name;

		Monster->Status.ID = (*MonsterTableRow).ID;
		Monster->Status.MonsterClass = (*MonsterTableRow).MonsterClass;
		Monster->Status.AttackCount = (*MonsterTableRow).AttackCount;
		Monster->Status.PatternID = (*MonsterTableRow).PatternID;
		Monster->Status.bHasCharging = (*MonsterTableRow).bHasCharging;
		Monster->Status.Name = (*MonsterTableRow).Name;
		Monster->Status.Level = (*MonsterTableRow).Level;
		Monster->Status.CurHP = (*MonsterTableRow).CurHP;
		Monster->Status.MaxHP = (*MonsterTableRow).MaxHP;
		Monster->Status.CurGroggy = (*MonsterTableRow).CurGroggy;
		Monster->Status.MaxGroggy = (*MonsterTableRow).MaxGroggy;
		Monster->Status.Damage = (*MonsterTableRow).Damage;
		Monster->Status.Deffence = (*MonsterTableRow).Deffence;
		Monster->Status.NormalSpeed = (*MonsterTableRow).NormalSpeed;
		Monster->Status.FollowSpeed = (*MonsterTableRow).FollowSpeed;
		Monster->Status.DetectRange = (*MonsterTableRow).DetectRange;
		Monster->Status.AttackRange = (*MonsterTableRow).AttackRange;
		Monster->Status.Exp = (*MonsterTableRow).Exp;
		Monster->Status.Gold = (*MonsterTableRow).Gold;
		Monster->Status.ByProductID = (*MonsterTableRow).ByProductID;
		Monster->Status.RewardID = (*MonsterTableRow).RewardID;
	}
}

void AMonsterManager::SpawnMonsterInLocation(int32 MonsterID, FVector Location)
{
	AMonster* Monster = CreateMonsterActor(MonsterID);

	Monster->SetActorLocation(Location);
}

AMonster* AMonsterManager::CreateMonsterActor(int32 MonsterID)
{
	auto Monster = GetWorld()->SpawnActor<AMonster>(MonsterMaps[MonsterID]);

	SetMonsterData(Monster, MonsterID);
	Monster->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	return Monster;
}
