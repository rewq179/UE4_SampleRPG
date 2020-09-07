// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/NpcManager.h"
#include "Manager/GameManager.h"
#include "Manager/QuestManager.h"

#include "Npc/NpcController.h"


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
		NpcDataMap[NpcID].NpcClass = (*NpcTableData).NpcClass;
		NpcDataMap[NpcID].Name = (*NpcTableData).Name;
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


void ANpcManager::CheckQuestSymbol(int32 NpcID) // 퀘스트 심볼을 확인해본다.
{
	if (GameManager->QuestManager->IsExclamationSymbol(NpcMap[NpcID]->QuestID)) // Npc가 보유한 퀘스트 ID를 모두 체크함.
	{
		SetQuestSymbol(NpcID, ESymbolType::EST_Exclamation);
	}
}

void ANpcManager::SetQuestSymbol(int32 NpcID, ESymbolType SymbolType)
{
	NpcMap[NpcID]->SetActiveSymbol(SymbolType);
}

void ANpcManager::CheckQuestSymbolAllNPC()
{
	for (auto& Map : NpcMap)
	{
		CheckQuestSymbol(Map.Key);
	}
}
