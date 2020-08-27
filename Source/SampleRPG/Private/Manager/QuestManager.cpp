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

FQuestTable AQuestManager::GetQuestData(int32 QuestID)
{
	return QuestMap[QuestID];
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
		FVector Location = GameManager->NpcManager->NpcMap[Quest.NpcID]->GetActorLocation();
		float x = Location.X;
		float y = Location.Y;
			float z = Location.Z;

			UE_LOG(LogTemp, Log, TEXT("%f %f %f"), x, y, z);

		Location += FVector(0.f, 210.f, -78.f);

		GameManager->ItemManager->SpawnItemActor(Quest.TargetID, Quest.MaxCount, Location);
	}

	GameManager->NpcManager->SetNpcSymbol(ESymbolType::EQT_None, Quest.NpcID);
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

	SetSymbol(ESymbolType::EQT_None, Quest.NpcID);
	GameManager->NpcManager->CheckNpcSymbol(Quest.NpcID);

	if (Quest.QuestID == 3) // 듀토리얼 종료
	{

	}
}

void AQuestManager::SetSymbol(ESymbolType SymbolType, int32 QuestID)
{
	int32 NpcID = QuestMap[QuestID].NpcID;

	GameManager->NpcManager->SetNpcSymbol(SymbolType, NpcID);
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

bool AQuestManager::IsPrerequisiteMeet(int32 QuestID) // 선행 조건을 만족했는지?
{
	TMap<int32, FQuestTable> Quests = MainPlayer->PlayerQuest->Quests;

	if (!Quests.Contains(QuestID)) // 퀘스트를 보유하지 않았고
	{
		int32 PreQuestID = GetPreQuestID(QuestID);

		if (PreQuestID == -1) // 선행 퀘스트가 없다? 그럼 퀘스트를 띄어준다.
		{
			return true;
		}

		else // 선행 퀘스트가 있다?
		{
			if (Quests.Contains(PreQuestID) && Quests[PreQuestID].bIsClear)
			{
				return true;
			}
		}
	}

	return false;
}
