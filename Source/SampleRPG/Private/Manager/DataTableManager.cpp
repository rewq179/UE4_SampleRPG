// Fill out your copyright notice in the Description page of Project Settings.

#include "DataTableManager.h"
#include "UObject/ConstructorHelpers.h"



// Sets default values
ADataTableManager::ADataTableManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DialogueTable(TEXT("DataTable'/Game/DataTable/DialogueTable.DialogueTable'"));
	if (DialogueTable.Succeeded())
	{
		DialogueTableData = DialogueTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DungeonRawTable(TEXT("DataTable'/Game/DataTable/DungeonRawTable.DungeonRawTable'"));
	if (DungeonRawTable.Succeeded())
	{
		DungeonRawTableData = DungeonRawTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(TEXT("DataTable'/Game/DataTable/ItemTable.ItemTable'"));
	if (ItemTable.Succeeded())
	{
		ItemTableData = ItemTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterTable(TEXT("DataTable'/Game/DataTable/MonsterTable.MonsterTable'"));
	if (MonsterTable.Succeeded())
	{
		MonsterTableData = MonsterTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> NpcTable(TEXT("DataTable'/Game/DataTable/NpcTable.NpcTable'"));
	if (NpcTable.Succeeded())
	{
		NpcTableData = NpcTable.Object;
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> PatternTable(TEXT("DataTable'/Game/DataTable/PatternTable.PatternTable'"));
	if (PatternTable.Succeeded())
	{
		PatternTableData = PatternTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> QuestTable(TEXT("DataTable'/Game/DataTable/QuestTable.QuestTable'"));
	if (QuestTable.Succeeded())
	{
		QuestTableData = QuestTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> RewardRawTable(TEXT("DataTable'/Game/DataTable/RewardRawTable.RewardRawTable'"));
	if (RewardRawTable.Succeeded())
	{
		RewardRawTableData = RewardRawTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillTable(TEXT("DataTable'/Game/DataTable/SkillTable.SkillTable'"));
	if (SkillTable.Succeeded())
	{
		SkillTableData = SkillTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> TriggerRawTable(TEXT("DataTable'/Game/DataTable/TriggerRawTable.TriggerRawTable'"));
	if (TriggerRawTable.Succeeded())
	{
		TriggerRawTableData = TriggerRawTable.Object;
	}
}

FText ADataTableManager::GetDialogueText(int32 DialogueID)
{
	return GetDialogueTableData(DialogueID)->DialogueText;
}

FText ADataTableManager::GetObjectName(EObjectType ObjectType, int32 ID)
{
	switch (ObjectType)
	{
	case EObjectType::EOT_Item:
		return GetItemTableData(ID)->Name;

	case EObjectType::EOT_Npc:
		return GetNpcTableData(ID)->Name;

	case EObjectType::EOT_Quest:
		return GetQuestTableData(ID)->Name;

	case EObjectType::EOT_Monster:
		return GetMonsterTableData(ID)->Name;

	default:
		return FText();
	}
}