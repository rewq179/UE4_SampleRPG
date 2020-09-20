// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/DungeonManager.h"
#include "Manager/GameManager.h"
#include "Manager/RGameInstance.h"
#include "Manager/MonsterManager.h"
#include "Manager/LevelManager.h"

#include "Monster.h"
#include "MainPlayer.h"

#include "Dungeon/BlockingTrigger.h"
#include "Dungeon/SpawnPoint.h"

#include "Engine/World.h"
#include "TimerManager.h"


// Sets default values
ADungeonManager::ADungeonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelectID = 0;
	ReviveTime = 10;
	ReviveCount = 1;
}

// Called when the game starts or when spawned
void ADungeonManager::BeginPlay()
{
	Super::BeginPlay();
}

void ADungeonManager::LoadLevel(FString LevelName)
{
	MainPlayer->SaveGame("");

	LevelManager->LoadLevel(LevelName);
}

/* Getter & Setter */

FDungeonTable ADungeonManager::GetDungeonData(int32 DungeonID)
{
	return DungeonDataMap[DungeonID];
}

void ADungeonManager::SetDungeonData(int32 DungeonID)
{
	FDungeonRawTable* DungeonRawTableData = GameManager->DataTableManager->GetDungeonRawTableData(DungeonID);

	if (DungeonRawTableData)
	{
		DungeonDataMap[DungeonID].DungeonID = (*DungeonRawTableData).DungeonID;
		DungeonDataMap[DungeonID].Name = (*DungeonRawTableData).Name;
		DungeonDataMap[DungeonID].LevelName = (*DungeonRawTableData).LevelName;

		StringToIntDungeonArray(DungeonID, (*DungeonRawTableData).TriggerID, 0);
		StringToIntDungeonArray(DungeonID, (*DungeonRawTableData).RewardID, 1);
		StringToIntDungeonArray(DungeonID, (*DungeonRawTableData).ItemID, 2);
	}
}

void ADungeonManager::SetDungeonDataAll()
{
	for (int32 Count = 0; Count < DungeonDataMap.Num(); Count++)
	{
		SetDungeonData(Count);
	}
}

void ADungeonManager::StringToIntDungeonArray(int32 DungeonID, FString Data, int32 Column)
{
	TArray<FString> StringID;

	FString DataString(Data);
	DataString.ParseIntoArray(StringID, TEXT("/"), true);

	for (auto ID : StringID)
	{
		switch (Column)
		{
		case 0:
			DungeonDataMap[DungeonID].TriggerIDs.Add(FCString::Atoi(*ID));
			break;


		case 1:
			DungeonDataMap[DungeonID].RewardIDs.Add(FCString::Atoi(*ID));
			break;


		case 2:
			DungeonDataMap[DungeonID].ItemIDs.Add(FCString::Atoi(*ID));
			break;

		default:
			break;
		}
	}
}

FTriggerTable ADungeonManager::GetTriggerData(int32 TriggerID)
{
	return TriggerDataMap[TriggerID];
}

void ADungeonManager::SetTriggerData(int32 TriggerID)
{
	FTriggerRawTable* TriggerRawTableData = GameManager->DataTableManager->GetTriggerRawTableData(TriggerID);

	if (TriggerRawTableData)
	{
		TriggerDataMap[TriggerID].TriggerID = (*TriggerRawTableData).TriggerID;
		TriggerDataMap[TriggerID].TriggerClass = (*TriggerRawTableData).TriggerClass;
		TriggerDataMap[TriggerID].TriggerType = (*TriggerRawTableData).TriggerType;
		
		StringToIntTriggerArray(TriggerID, (*TriggerRawTableData).BlockID, 0);
		StringToIntTriggerArray(TriggerID, (*TriggerRawTableData).MonsterID, 1);
		StringToIntTriggerArray(TriggerID, (*TriggerRawTableData).PointID, 2);

		TriggerDataMap[TriggerID].ID_0 = (*TriggerRawTableData).ID_0;
		TriggerDataMap[TriggerID].CurCount_0 = (*TriggerRawTableData).CurCount_0;
		TriggerDataMap[TriggerID].MaxCount_0 = (*TriggerRawTableData).MaxCount_0;
		TriggerDataMap[TriggerID].ID_1 = (*TriggerRawTableData).ID_1;
		TriggerDataMap[TriggerID].CurCount_1 = (*TriggerRawTableData).CurCount_1;
		TriggerDataMap[TriggerID].MaxCount_1 = (*TriggerRawTableData).MaxCount_1;

	}
}


void ADungeonManager::StringToIntTriggerArray(int32 TriggerID, FString Data, int32 Column)
{
	TArray<FString> StringID;

	FString DataString(Data);
	DataString.ParseIntoArray(StringID, TEXT("/"), true);

	for (auto ID : StringID)
	{
		switch (Column)
		{
		case 0:
			TriggerDataMap[TriggerID].BlockIDs.Add(FCString::Atoi(*ID));
			break;


		case 1:
			TriggerDataMap[TriggerID].MonsterIDs.Add(FCString::Atoi(*ID));
			break;


		case 2:
			TriggerDataMap[TriggerID].PointIDs.Add(FCString::Atoi(*ID));
			break;

		default:
			break;
		}
	}
}

void ADungeonManager::SetTriggerDataAll()
{
	for (int32 Count = 0; Count < TriggerDataMap.Num(); Count++)
	{
		SetTriggerData(Count);
	}


}

/* Dungeon Playing Data */

void ADungeonManager::SetReviveData()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADungeonManager::DecreaseReviveTime, 1.f, true, 0.f);

	SetActiveReviveHUD();
}

void ADungeonManager::DecreaseReviveTime()
{
	LifeTime += GetWorldTimerManager().GetTimerElapsed(TimerHandle);

	if (LifeTime > ReviveTime)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		LifeTime = 0.f;

		LoadLevel("Default");
	}
}

void ADungeonManager::StopTimer()
{
	LifeTime = 0.f;

	GetWorldTimerManager().ClearTimer(TimerHandle);
}

/* Add Dungeon's Spawn Point, Block */

void ADungeonManager::AddSpawnPoint(int32 Index, ASpawnPoint* SpawnPoint)
{
	if (SpawnPointMap.Find(Index))
	{
		return;
	}

	SpawnPointMap.Add(Index, SpawnPoint);
}

void ADungeonManager::AddBlockingTrigger(int32 Index, ABlockingTrigger* BlockingTrigger)
{
	if (BlockingTriggerMap.Find(Index))
	{
		return;
	}

	BlockingTriggerMap.Add(Index, BlockingTrigger);
}

/* Dungeon's Trigger */

void ADungeonManager::SetDungeonTrigger()
{
	CurDungeonID = GameManager->GameInstance->DungeonID;

	for (auto TriggerID : DungeonDataMap[CurDungeonID].TriggerIDs)
	{
		CurTriggerMaps.Add(GetTriggerData(TriggerID));
	}
}

void ADungeonManager::CheckTrigger(int32 TargetID)
{
	if (!CurTriggerMaps[SelectID].bOnGoing)
	{
		CurTriggerMaps[SelectID].bOnGoing = true;
	}

	ActiveInstantTrigger();

	if (CurTriggerMaps[SelectID].ID_0 == TargetID)
	{
		CurTriggerMaps[SelectID].CurCount_0++;

		if (CanActiveTrigger()) // 조건 검사후 만족시 트리거 발동
		{
			SetActiveTrigger();
		}
	}
}

void ADungeonManager::ActiveInstantTrigger()
{
	if (CurTriggerMaps[SelectID].TriggerClass == ETriggerClass::ETC_Instant) // 즉시 발동 트리거면 바로 재생
	{
		SetActiveTrigger();
	}
}


bool ADungeonManager::CanActiveTrigger()
{
	if (CurTriggerMaps[SelectID].bOnGoing && !CurTriggerMaps[SelectID].bIsClear)
	{
		if (CurTriggerMaps[SelectID].CurCount_0 >= CurTriggerMaps[SelectID].MaxCount_0)
		{
			return true;
		}
	}

	return false;
}

/* Active Trigger */

void ADungeonManager::SetActiveTrigger()
{
	CurTriggerMaps[SelectID].bIsClear = true;
	CurTriggerMaps[SelectID].bOnGoing = false;

	switch (CurTriggerMaps[SelectID].TriggerType)
	{
	case ETriggerType::ETT_Block:
		for (int32 Index = 0; Index < CurTriggerMaps[SelectID].BlockIDs.Num(); Index++)
		{
			BlockingTriggerMap[CurTriggerMaps[SelectID].BlockIDs[Index]]->SetOverlap();
		}
		break;

	case ETriggerType::ETT_Spawn:
		for (int32 Index = 0; Index < CurTriggerMaps[SelectID].MonsterIDs.Num(); Index++)
		{
			MonsterManager->SpawnMonsterInLocation(CurTriggerMaps[SelectID].MonsterIDs[Index], SpawnPointMap[CurTriggerMaps[SelectID].PointIDs[Index]]->GetActorLocation());
		}
		break;

	case ETriggerType::ETT_Clear:
		GiveClearReward();
		break;
	}

	if (SelectID < CurTriggerMaps.Num())
	{
		SelectID++;

		ActiveInstantTrigger();
	}
}

void ADungeonManager::GiveClearReward()
{
	int32 Rand = FMath::RandRange(0, DungeonDataMap[CurDungeonID].RewardIDs.Num() - 1);

	GameManager->ItemManager->GetItemInRewardBox(DungeonDataMap[CurDungeonID].RewardIDs[Rand], MainPlayer->GetActorLocation());

	SetActiveDungeonClearHUD();
}