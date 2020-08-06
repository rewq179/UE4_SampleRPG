// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerQuest.h"
#include "MainPlayer.h"

#include "Npc/NpcController.h"
#include "Engine/World.h"

// Sets default values
APlayerQuest::APlayerQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerQuest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerQuest::AddQuest()
{
	Quests.Add(RecentQuest.QuestID, RecentQuest);

	Quests.KeySort([](int32 A, int32 B) {
		return A < B; 
	});

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


bool APlayerQuest::CheckQuest()
{
	// 퀘스트 상자박스 클릭 -> 해당 NPC의 0번째 퀘스트 부터 검사함
	// 퀘스트를 소유했는가? -> Yes : 완료했는가? 다음 퀘스트 검사
	//                   -> No : 부여

	for (int32 Index = 0; Index < InteractNPC->QuestID.Num(); Index++)
	{
		int32 QuestID = InteractNPC->QuestID[Index];

		if (!IsQuestExist(QuestID)) // 퀘스트를 보유하지 않았고
		{
			int32 PreQuestID = QuestManager->GetPreQuestID(QuestID);

			if (PreQuestID == -1) // 선행 퀘스트가 없다? 그럼 퀘스트를 띄어준다.
			{
				RecentQuest = QuestManager->GetQuestData(QuestID);

				return true;
			}

			else // 선행 퀘스트가 있다?
			{
				if (IsPreQuestClear(PreQuestID))
				{
					RecentQuest = QuestManager->GetQuestData(QuestID);

					return true;
				}
			}
		}

		else
		{
			if (!Quests[QuestID].bIsClear)
			{
				return false;
			}
		}
	}

	return false;
}

bool APlayerQuest::IsQuestExist(int32 QuestID)
{
	if (Quests.Contains(QuestID))
	{
		return true;
	}

	return false;
}

bool APlayerQuest::IsPreQuestClear(int32 QuestID)
{
	if (IsQuestExist(Quests[QuestID].PreQuest) && Quests[Quests[QuestID].PreQuest].bIsClear)
	{
		return true;
	}

	return false;
}

FQuestTable APlayerQuest::GetQuestData(int32 QuestID)
{
	return Quests[QuestID];
}
