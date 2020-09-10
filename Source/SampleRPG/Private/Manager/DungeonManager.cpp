// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonManager.h"
#include "Manager/GameManager.h"

#include "SpawnPoint.h"

// Sets default values
ADungeonManager::ADungeonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonManager::BeginPlay()
{
	Super::BeginPlay();
	
}

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


		StringToIntArray(DungeonID, (*DungeonRawTableData).TriggerID);
	}
}

void ADungeonManager::SetDungeonDataAll()
{
	for (int32 Count = 0; Count < DungeonDataMap.Num(); Count++)
	{
		SetDungeonData(Count);
	}
}

void ADungeonManager::StringToIntArray(int32 DungeonID, FString Data)
{
	TArray<FString> StringID;

	FString DataString(Data);
	DataString.ParseIntoArray(StringID, TEXT("/"), true);

	for (auto ID : StringID)
	{
		DungeonDataMap[DungeonID].TriggerIDs.Add(FCString::Atoi(*ID));
	}
}

void ADungeonManager::AddSpawnPoint(int32 Index, ASpawnPoint* SpawnPoint)
{
	if (SpawnPointMap.Find(Index))
	{
		return;
	}

	SpawnPointMap.Add(Index, SpawnPoint);
}



void ADungeonManager::CheckTrigger(int32 TriggerID)
{

}

void ADungeonManager::CheckTriggerAll()
{
	
}
//
//void ADungeonManager::SpawnDungeonInLocation()
//{
//
//}
