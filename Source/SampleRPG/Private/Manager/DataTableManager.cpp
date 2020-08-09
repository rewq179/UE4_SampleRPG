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

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(TEXT("DataTable'/Game/DataTable/ItemTable.ItemTable'"));

	if (ItemTable.Succeeded())
	{
		ItemTableData = ItemTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> QuestTable(TEXT("DataTable'/Game/DataTable/QuestTable.QuestTable'"));

	if (QuestTable.Succeeded())
	{
		QuestTableData = QuestTable.Object;
	}

}

// Called when the game starts or when spawned
void ADataTableManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADataTableManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FDialogueTable* ADataTableManager::GetDialogueData(int32 DialogueID)
{
	return DialogueTableData->FindRow<FDialogueTable>(FName(*(FString::FormatAsNumber(DialogueID))), FString(""));
}

FNpcTable* ADataTableManager::GetNpcData(int32 NpcID)
{
	return NpcTableData->FindRow<FNpcTable>(FName(*(FString::FormatAsNumber(NpcID))), FString(""));
}

FMonsterTable* ADataTableManager::GetMonsterData(int32 MonsterID)
{
	return MonsterTableData->FindRow<FMonsterTable>(FName(*(FString::FormatAsNumber(MonsterID))), FString(""));
}

FItemTable* ADataTableManager::GetItemData(int32 ItemID)
{
	return ItemTableData->FindRow<FItemTable>(FName(*(FString::FormatAsNumber(ItemID))), FString(""));
}

FQuestTable* ADataTableManager::GetQuestData(int32 QuestID)
{
	return QuestTableData->FindRow<FQuestTable>(FName(*(FString::FormatAsNumber(QuestID))), FString(""));
}

FText ADataTableManager::GetDialogueText(int32 DialogueID)
{
	return GetDialogueData(DialogueID)->DialogueText;
}

FText ADataTableManager::GetDataName(EDataType DataType, int32 ID)
{
	switch (DataType)
	{
	case EDataType::EDT_Item:
		return GetItemData(ID)->Name;

	case EDataType::EDT_Npc:
		return GetNpcData(ID)->Name;

	case EDataType::EDT_Quest:
		return GetQuestData(ID)->Name;

	case EDataType::EDT_Monster:
		return GetMonsterData(ID)->Name;

	default:
		return FText();
	}
}