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

	static ConstructorHelpers::FObjectFinder<UDataTable> NpcTable(TEXT("DataTable'/Game/DataTable/NpcTable.NpcTable'"));
	if (NpcTable.Succeeded())
	{
		NpcTableData = NpcTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterTable(TEXT("DataTable'/Game/DataTable/MonsterTable.MonsterTable'"));
	if (MonsterTable.Succeeded())
	{
		MonsterTableData = MonsterTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> PatternRawTable(TEXT("DataTable'/Game/DataTable/PatternRawTable.PatternRawTable'"));
	if (PatternRawTable.Succeeded())
	{
		PatternRawTableData = PatternRawTable.Object;
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(TEXT("DataTable'/Game/DataTable/ItemTable.ItemTable'"));
	if (ItemTable.Succeeded())
	{
		ItemTableData = ItemTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> RewardRawTable(TEXT("DataTable'/Game/DataTable/RewardRawTable.RewardRawTable'"));
	if (RewardRawTable.Succeeded())
	{
		RewardRawTableData = RewardRawTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> QuestTable(TEXT("DataTable'/Game/DataTable/QuestTable.QuestTable'"));
	if (QuestTable.Succeeded())
	{
		QuestTableData = QuestTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillTable(TEXT("DataTable'/Game/DataTable/SkillTable.SkillTable'"));
	if (SkillTable.Succeeded())
	{
		SkillTableData = SkillTable.Object;
	}
}

FDialogueTable* ADataTableManager::GetDialogueTableData(int32 DialogueID)
{
	return DialogueTableData->FindRow<FDialogueTable>(FName(*(FString::FormatAsNumber(DialogueID))), FString(""));
}

FNpcTable* ADataTableManager::GetNpcTableData(int32 NpcID)
{
	return NpcTableData->FindRow<FNpcTable>(FName(*(FString::FormatAsNumber(NpcID))), FString(""));
}

FMonsterTable* ADataTableManager::GetMonsterTableData(int32 MonsterID)
{
	return MonsterTableData->FindRow<FMonsterTable>(FName(*(FString::FormatAsNumber(MonsterID))), FString(""));
}

FPatternRawTable* ADataTableManager::GetPatternRawTableData(int32 PatternID)
{
	return PatternRawTableData->FindRow<FPatternRawTable>(FName(*(FString::FormatAsNumber(PatternID))), FString(""));
}

FItemTable* ADataTableManager::GetItemTableData(int32 ItemID)
{
	return ItemTableData->FindRow<FItemTable>(FName(*(FString::FormatAsNumber(ItemID))), FString(""));
}

FRewardRawTable* ADataTableManager::GetRewardRawTableData(int32 RewardID)
{
	return RewardRawTableData->FindRow<FRewardRawTable>(FName(*(FString::FormatAsNumber(RewardID))), FString(""));
}


FQuestTable* ADataTableManager::GetQuestTableData(int32 QuestID)
{
	return QuestTableData->FindRow<FQuestTable>(FName(*(FString::FormatAsNumber(QuestID))), FString(""));
}

FSkillTable* ADataTableManager::GetSkillTableData(int32 SkillID)
{
	return SkillTableData->FindRow<FSkillTable>(FName(*(FString::FormatAsNumber(SkillID))), FString(""));
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