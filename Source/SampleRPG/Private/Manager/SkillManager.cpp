// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillManager.h"

#include "Manager/GameManager.h"

// Sets default values
ASkillManager::ASkillManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FSkillTable ASkillManager::GetSkillData(int32 SkillID)
{
	return SkillMap[SkillID];
}


void ASkillManager::SetSkillData(int32 SkillID)
{
	FSkillTable* SkillTableData = GameManager->DataTableManager->GetSkillTableData(SkillID);

	if (SkillTableData)
	{
		SkillMap[SkillID].SkillID = (*SkillTableData).SkillID;
		SkillMap[SkillID].SkillID = (*SkillTableData).SkillID;
		SkillMap[SkillID].SkillClass = (*SkillTableData).SkillClass;
		SkillMap[SkillID].Name = (*SkillTableData).Name;
		SkillMap[SkillID].DurationTime = (*SkillTableData).DurationTime;
		SkillMap[SkillID].IncPerDamage = (*SkillTableData).IncPerDamage;
		SkillMap[SkillID].IncPerDeffence = (*SkillTableData).IncPerDeffence;
		SkillMap[SkillID].CurHP = (*SkillTableData).CurHP;
		SkillMap[SkillID].PerHP = (*SkillTableData).PerHP;
		SkillMap[SkillID].CurST = (*SkillTableData).CurST;
		SkillMap[SkillID].PerST = (*SkillTableData).PerST;
		SkillMap[SkillID].PerShield = (*SkillTableData).PerShield;
		SkillMap[SkillID].PerSpeed = (*SkillTableData).PerSpeed;

	}
}

void ASkillManager::SetSkillDataAll()
{
	if (SkillMap.Num() > 0 && GameManager)
	{
		for (int32 Count = 0; Count < SkillMap.Num(); Count++)
		{
			SetSkillData(Count);
		}
	}
}

float ASkillManager::GetSkillDamage(int32 SkillID)
{
	auto SkillData = GetSkillData(SkillID);

	switch (SkillData.StatusType)
	{
	case EStatusType::EST_None:
		return -1;

	case EStatusType::EST_CurHp:
		return SkillData.CurHP;

	case EStatusType::EST_PerHP:
		return SkillData.PerHP;

	case EStatusType::EST_CurST:
		return SkillData.CurST;

	case EStatusType::EST_PerST:
		return SkillData.PerST;

	default:
		return -1;
	}
}
