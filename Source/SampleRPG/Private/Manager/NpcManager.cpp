// Fill out your copyright notice in the Description page of Project Settings.

#include "NpcManager.h"
#include "Npc/NpcController.h"
#include "Manager/GameManager.h"

// Sets default values
ANpcManager::ANpcManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FNpcTable ANpcManager::GetNpcData(int32 NpcID)
{
	return NpcDataMap[NpcID];
}

void ANpcManager::SetNpcData(int32 NpcID)
{
	FNpcTable* NpcTableData = GameManager->DataTableManager->GetNpcTableData(NpcID);

	if (NpcTableData)
	{
		NpcDataMap[NpcID].NpcID = (*NpcTableData).NpcID;
		NpcDataMap[NpcID].Name = (*NpcTableData).Name;
		NpcDataMap[NpcID].bHasItem = (*NpcTableData).bHasItem;
		NpcDataMap[NpcID].bHasQuest = (*NpcTableData).bHasQuest;
		NpcDataMap[NpcID].DialogueID = (*NpcTableData).DialogueID;
		NpcDataMap[NpcID].ItemID = (*NpcTableData).ItemID;
		NpcDataMap[NpcID].QuestID = (*NpcTableData).QuestID;
	}
}

void ANpcManager::SetNpcDataAll()
{
	for (int32 Count = 0; Count < NpcDataMap.Num(); Count++)
	{
		SetNpcData(Count);
	}
}

void ANpcManager::SetNpcSymbol(ESymbolType SymbolType, int32 NpcID)
{
	NpcMap[NpcID]->SetActiveSymbol(SymbolType);
}

void ANpcManager::CheckNpcSymbol(int32 NpcID) // 퀘스트 심볼을 확인해본다.
{
	if (GameManager->QuestManager->IsExclamationSymbol(NpcMap[NpcID]->QuestID))
	{
		SetNpcSymbol(ESymbolType::EST_Exclamation, NpcID);
	}
}

void ANpcManager::CheckNpcSymbolAll()
{
	for (auto& Map : NpcMap)
	{
		CheckNpcSymbol(Map.Key);
	}
}
