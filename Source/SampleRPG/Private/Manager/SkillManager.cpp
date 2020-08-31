// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillManager.h"

#include "Manager/GameManager.h"

// Sets default values
ASkillManager::ASkillManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASkillManager::SetAllSkillData()
{
	if (SkillMap.Num() > 0 && GameManager)
	{
		for (int32 Count = 0; Count < SkillMap.Num(); Count++)
		{
			SetSkillData(Count);
		}
	}
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

FSkillTable ASkillManager::GetSkillData(int32 SkillID)
{
	return SkillMap[SkillID];
}
