// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"
#include "Manager/GameManager.h"
#include "Manager/ItemManager.h"
#include "Player/MainPlayer.h"
#include "Player/PlayerQuest.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/QuestTable.QuestTable'"));

	if (DataTable.Succeeded())
	{
		QuestTable = DataTable.Object;
	}
}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (QuestTable && QuestMap.Num() > 0)
	{
		for (int32 Count = 0; Count < QuestMap.Num(); Count++)
		{
			SetQuestData(Count);
		}
	}
}

void AQuestManager::SetQuestData(int32 QuestID)
{
	FQuestTable* QuestTableRow = QuestTable->FindRow<FQuestTable>(FName(*(FString::FormatAsNumber(QuestID))), FString(""));

	if (QuestTableRow)
	{
		QuestMap[QuestID].QuestSubID = (*QuestTableRow).QuestSubID;
		QuestMap[QuestID].QuestClass = (*QuestTableRow).QuestClass;
		QuestMap[QuestID].QuestType = (*QuestTableRow).QuestType;
		QuestMap[QuestID].Name = (*QuestTableRow).Name;
		QuestMap[QuestID].Goal = (*QuestTableRow).Goal;
		QuestMap[QuestID].PreDialogueID = (*QuestTableRow).PreDialogueID;
		QuestMap[QuestID].PostDialogueID = (*QuestTableRow).PostDialogueID;
		QuestMap[QuestID].PreQuest = (*QuestTableRow).PreQuest;
		QuestMap[QuestID].bCanClear = (*QuestTableRow).bCanClear;
		QuestMap[QuestID].bIsClear = (*QuestTableRow).bIsClear;
		QuestMap[QuestID].NpcID = (*QuestTableRow).NpcID;
		QuestMap[QuestID].TargetID = (*QuestTableRow).TargetID;
		QuestMap[QuestID].Count = (*QuestTableRow).Count;
		QuestMap[QuestID].Exp = (*QuestTableRow).Exp;
		QuestMap[QuestID].Gold = (*QuestTableRow).Gold;
		QuestMap[QuestID].PreReward = (*QuestTableRow).PreReward;
		QuestMap[QuestID].PreRewardCount = (*QuestTableRow).PreRewardCount;
		QuestMap[QuestID].PostReward = (*QuestTableRow).PostReward;
		QuestMap[QuestID].PostRewardCount = (*QuestTableRow).PostRewardCount;
	}
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FQuestTable AQuestManager::GetQuestData(int32 QuestID)
{
	return QuestMap[QuestID];
}


UTexture2D* AQuestManager::GetRewardIcon(int32 ItemID)
{
	return GameManager->ItemManager->GetItemTableData(ItemID).Icon;
}

void AQuestManager::AcceptQuest()
{
	MainPlayer->PlayerQuest->AddQuest();
}

void AQuestManager::ClearQuest()
{

}