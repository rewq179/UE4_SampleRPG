// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerQuest.h"
#include "Player/MainPlayer.h"

#include "Manager/SaveGameManager.h"
#include "Manager/GameManager.h"

#include "Npc/NpcController.h"
#include "Engine/World.h"

// Sets default values
APlayerQuest::APlayerQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void APlayerQuest::AddQuest()
{
	RecentQuest.bIsAlreadyGive = true;
	Quests.Add(RecentQuest.QuestID, RecentQuest);

	/*Quests.KeySort([](int32 A, int32 B) {
		return A < B; 
	});*/

	SetQuestKey();
}

void APlayerQuest::SetQuestKey()
{
	QuestKey.Empty();

	for (auto& Quest : Quests)
	{
		QuestKey.Add(Quest.Key);
	}
}

void APlayerQuest::ClearQuest(int32 QuestID)
{
	if (Quests.Contains(QuestID))
	{
		Quests[QuestID].bCanClear = false;
		Quests[QuestID].bIsClear = true;
	}
}

void APlayerQuest::SetQuestTepList(bool bIsClearTep)
{
	QuestTepList.Empty();

	for (auto& Map : Quests)
	{
		FQuestTable Quest = Map.Value;

		if (Quest.bIsClear == bIsClearTep)
		{
			QuestTepList.Add(Map.Key);
		}
	}
}

bool APlayerQuest::IsClearQuestExist()
{
	for (int32 Index = 0; Index < InteractNPC->QuestID.Num(); Index++)
	{
		int32 QuestID = InteractNPC->QuestID[Index];

		if (Quests.Contains(QuestID) && Quests[QuestID].bCanClear && !Quests[QuestID].bIsClear)
		{
			RecentQuest = Quests[QuestID];

			return true;
		}
	}

	return false;
}

bool APlayerQuest::CanGetQuest()
{
	// 퀘스트 상자박스 클릭 -> 해당 NPC의 0번째 퀘스트 부터 검사함
	// 퀘스트를 소유했는가? -> Yes : 완료했는가? 다음 퀘스트 검사
	//                   -> No : 부여
	for (int32 Index = 0; Index < InteractNPC->QuestID.Num(); Index++)
	{
		int32 QuestID = InteractNPC->QuestID[Index];

		if (QuestManager->IsPrerequisiteMeet(QuestID))
		{
			RecentQuest = QuestManager->GetQuestData(QuestID);

			return true;
		}
	}
	
	return false;
}

FQuestTable APlayerQuest::GetQuestData(int32 QuestID)
{
	return Quests[QuestID];
}

void APlayerQuest::IncreaseCount(EQuestType QuestType, int32 TargetID, int32 Count)
{
	for (int32 Index = 0; Index < QuestKey.Num(); Index++)
	{
		if (Quests[QuestKey[Index]].QuestType == QuestType && Quests[QuestKey[Index]].TargetID == TargetID)
		{
			Quests[QuestKey[Index]].CurCount += Count;

			if (Quests[QuestKey[Index]].CurCount >= Quests[QuestKey[Index]].MaxCount)
			{
				Quests[QuestKey[Index]].bCanClear = true;

				if (MainPlayer->GameManager->LevelType != ELevelType::ELT_Dungeon)
				{
					QuestManager->NpcManager->SetQuestSymbol(Quests[QuestKey[Index]].NpcID, ESymbolType::EST_Question);
				}
			}
		}
	}
}

// Save & Load GameData //

void APlayerQuest::SaveQuestData(class USaveGameManager* SaveGameInstance)
{
	for (auto QuestID : QuestKey)
	{
		if (Quests[QuestID].bIsClear)
		{
			SaveGameInstance->PlayerQuestMap.Add(QuestID, true);
		}

		else
		{
			SaveGameInstance->PlayerQuestMap.Add(QuestID, false);
			SaveGameInstance->PlayerQuestCountMap.Add(QuestID, Quests[QuestID].CurCount);
		}
	}
}

void APlayerQuest::LoadQuestData(class USaveGameManager* LoadGameInstance)
{
	if (QuestManager)
	{
		Quests.Empty();
		QuestKey.Empty();

		for (auto Quest : LoadGameInstance->PlayerQuestMap)
		{
			RecentQuest = QuestManager->GetQuestData(Quest.Key);
			RecentQuest.bIsAlreadyGive = true;
			RecentQuest.bIsClear = Quest.Value;

			if (RecentQuest.bIsClear)
			{
				RecentQuest.CurCount = RecentQuest.MaxCount;
			}

			else
			{
				RecentQuest.CurCount = LoadGameInstance->PlayerQuestCountMap[Quest.Key];

				if (RecentQuest.CurCount >= RecentQuest.MaxCount)
				{
					RecentQuest.CurCount = RecentQuest.MaxCount;

					RecentQuest.bCanClear = true;

					if (MainPlayer->GameManager->LevelType != ELevelType::ELT_Dungeon)
					{
						QuestManager->NpcManager->SetQuestSymbol(RecentQuest.NpcID, ESymbolType::EST_Question);
					}
				}
			}

			AddQuest();
		}
	}
}