// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"

#include "Manager/GameManager.h"
#include "Player/MainPlayer.h"
#include "Player/PlayerQuest.h"
#include "Player/Inventory.h"

#include "Npc/NpcController.h"

#include "UObject/ConstructorHelpers.h"

/**
 * Player�� Quest, Npc�� ������ Quest, Hud�� Quest ȭ����� �����ϴ� Ŭ������.
 * ���� ��� QuestData�� ������ ������ �ִ�.
*/

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FQuestTable AQuestManager::GetQuestData(int32 QuestID)
{
	return QuestMap[QuestID];
}

void AQuestManager::SetQuestData(int32 QuestID)
{
	FQuestTable* QuestTableData = GameManager->DataTableManager->GetQuestTableData(QuestID);

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
		QuestMap[QuestID].bIsLastQuest = (*QuestTableData).bIsLastQuest;
		QuestMap[QuestID].bIsAlreadyGive = (*QuestTableData).bIsAlreadyGive;
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

void AQuestManager::SetQuestDataAll()
{
	if (QuestMap.Num() > 0 && GameManager)
	{
		for (int32 Count = 0; Count < QuestMap.Num(); Count++)
		{
			SetQuestData(Count);
		}
	}
}


UTexture2D* AQuestManager::GetRewardIcon(int32 ItemID)
{
	return GameManager->ItemManager->GetItemData(ItemID).Icon;
}

void AQuestManager::AcceptQuest(FQuestTable Quest)
{
	MainPlayer->PlayerQuest->AddQuest();

	if (Quest.PreRewardCount > 0)
	{
		AItem* PreItem = GameManager->ItemManager->CreateItemActor(Quest.PreReward, Quest.PreRewardCount);

		MainPlayer->Inventory->AddItem(PreItem);
	}

	if (Quest.QuestID == 2)
	{
		FVector Location = NpcManager->NpcMap[Quest.NpcID]->GetActorLocation();
	
		Location += FVector(0.f, 210.f, -78.f);

		GameManager->ItemManager->SpawnItemActor(Quest.TargetID, Quest.MaxCount, Location);
	}

	NpcManager->SetQuestSymbol(Quest.NpcID, ESymbolType::EST_None);
}

void AQuestManager::ClearQuest(FQuestTable Quest)
{
	MainPlayer->PlayerQuest->ClearQuest(Quest.QuestID);
	MainPlayer->PlayerStatus->AddExp(Quest.Exp);
	MainPlayer->Inventory->AddGold(Quest.Gold);

	if (Quest.PostRewardCount > 0)
	{
		AItem* RewardItem = GameManager->ItemManager->CreateItemActor(Quest.PostReward, Quest.PostRewardCount);
		
		MainPlayer->Inventory->AddItem(RewardItem);
	}

	NpcManager->SetQuestSymbol(Quest.NpcID, ESymbolType::EST_None);
	NpcManager->CheckQuestSymbol(Quest.NpcID);
}

bool AQuestManager::IsExclamationSymbol(TArray<int32> QuestID)
{
	for (auto ID : QuestID)
	{
		if (IsPrerequisiteMeet(ID))
		{
			return true;
		}
	}

	return false;
}

bool AQuestManager::IsPrerequisiteMeet(int32 QuestID) // ���� ������ �����ߴ���?
{
	TMap<int32, FQuestTable> Quests = MainPlayer->PlayerQuest->Quests;

	if (!Quests.Contains(QuestID)) // ����Ʈ�� �������� �ʾҰ�
	{
		int32 PreQuestID = QuestMap[QuestID].PreQuest;

		if (PreQuestID == -1) // ���� ����Ʈ�� ����? �׷� ����Ʈ�� ����ش�.
		{
			return true;
		}

		else // ���� ����Ʈ�� �ִ�?
		{
			if (Quests.Contains(PreQuestID) && Quests[PreQuestID].bIsClear)
			{
				return true;
			}
		}
	}

	return false;
}
