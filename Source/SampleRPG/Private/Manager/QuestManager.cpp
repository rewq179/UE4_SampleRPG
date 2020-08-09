// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"
#include "Manager/GameManager.h"
#include "Manager/ItemManager.h"
#include "Player/MainPlayer.h"
#include "Player/PlayerQuest.h"
#include "Player/Inventory.h"
#include "Npc/NpcController.h"

#include "UObject/ConstructorHelpers.h"


// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
}
void AQuestManager::SetAllQuestData()
{
	if (QuestMap.Num() > 0 && GameManager)
	{
		for (int32 Count = 0; Count < QuestMap.Num(); Count++)
		{
			SetQuestData(Count);
		}
	}
}

void AQuestManager::SetQuestData(int32 QuestID)
{
	FQuestTable* QuestTableData = GameManager->DataTableManager->GetQuestData(QuestID);

	if (QuestTableData)
	{
		QuestMap[QuestID].QuestID = (*QuestTableData).QuestID;
		QuestMap[QuestID].QuestSubID = (*QuestTableData).QuestSubID;
		QuestMap[QuestID].QuestClass = (*QuestTableData).QuestClass;
		QuestMap[QuestID].QuestType = (*QuestTableData).QuestType;
		QuestMap[QuestID].Name = (*QuestTableData).Name;
		QuestMap[QuestID].Goal = (*QuestTableData).Goal;
		QuestMap[QuestID].PreDialogueID = (*QuestTableData).PreDialogueID;
		QuestMap[QuestID].PostDialogueID = (*QuestTableData).PostDialogueID;
		QuestMap[QuestID].PreQuest = (*QuestTableData).PreQuest;
		QuestMap[QuestID].bCanClear = (*QuestTableData).bCanClear;
		QuestMap[QuestID].bIsClear = (*QuestTableData).bIsClear;
		QuestMap[QuestID].NpcID = (*QuestTableData).NpcID;
		QuestMap[QuestID].TargetID = (*QuestTableData).TargetID;
		QuestMap[QuestID].CurCount = (*QuestTableData).CurCount;
		QuestMap[QuestID].MaxCount = (*QuestTableData).MaxCount;
		QuestMap[QuestID].Exp = (*QuestTableData).Exp;
		QuestMap[QuestID].Gold = (*QuestTableData).Gold;
		QuestMap[QuestID].PreReward = (*QuestTableData).PreReward;
		QuestMap[QuestID].PreRewardCount = (*QuestTableData).PreRewardCount;
		QuestMap[QuestID].PostReward = (*QuestTableData).PostReward;
		QuestMap[QuestID].PostRewardCount = (*QuestTableData).PostRewardCount;
	}
}

FQuestTable AQuestManager::GetQuestData(int32 QuestID)
{
	return QuestMap[QuestID];
}


UTexture2D* AQuestManager::GetRewardIcon(int32 ItemID)
{
	return GameManager->ItemManager->GetItemTableData(ItemID).Icon;
}

void AQuestManager::AcceptQuest(FQuestTable Quest)
{
	MainPlayer->PlayerQuest->AddQuest();

	if (Quest.PreRewardCount > 0)
	{
		AItem* PreItem = GameManager->ItemManager->CreateItemActor(Quest.PreReward, Quest.PreRewardCount);

		MainPlayer->Inventory->AddItem(PreItem);
	}
}

void AQuestManager::ClearQuest(FQuestTable Quest)
{
	MainPlayer->PlayerQuest->ClearQuest(Quest.QuestID);

	MainPlayer->AddExp(Quest.Exp);
	MainPlayer->Inventory->AddGold(Quest.Gold);

	if (Quest.PostRewardCount > 0)
	{
		AItem* RewardItem = GameManager->ItemManager->CreateItemActor(Quest.PostReward, Quest.PostRewardCount);
		
		MainPlayer->Inventory->AddItem(RewardItem);
	}
}

void AQuestManager::CheckSymbolMark(int32 NpcID) 
{
	ANpcController* Npc = GameManager->NpcManager->NpcMap[NpcID];

	Npc->SetActiveSymbol(ESymbolType::EQT_Question);
}
