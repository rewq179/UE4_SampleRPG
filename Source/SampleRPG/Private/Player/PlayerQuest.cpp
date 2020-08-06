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
	// ����Ʈ ���ڹڽ� Ŭ�� -> �ش� NPC�� 0��° ����Ʈ ���� �˻���
	// ����Ʈ�� �����ߴ°�? -> Yes : �Ϸ��ߴ°�? ���� ����Ʈ �˻�
	//                   -> No : �ο�

	for (int32 Index = 0; Index < InteractNPC->QuestID.Num(); Index++)
	{
		int32 QuestID = InteractNPC->QuestID[Index];

		if (!IsQuestExist(QuestID)) // ����Ʈ�� �������� �ʾҰ�
		{
			int32 PreQuestID = QuestManager->GetPreQuestID(QuestID);

			if (PreQuestID == -1) // ���� ����Ʈ�� ����? �׷� ����Ʈ�� ����ش�.
			{
				RecentQuest = QuestManager->GetQuestData(QuestID);

				return true;
			}

			else // ���� ����Ʈ�� �ִ�?
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
