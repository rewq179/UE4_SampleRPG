// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Player/MainPlayer.h"
#include "Engine/World.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
#pragma region Create Manager BP & Set 
	if (DataTableManagerBP)
	{
		DataTableManager = GetWorld()->SpawnActor<ADataTableManager>(DataTableManagerBP);

		if (DataTableManager)
		{
			DataTableManager->GameManager = this;
			DataTableManager->MainPlayer = MainPlayer;
			DataTableManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (DialogueManagerBP)
	{
		DialogueManager = GetWorld()->SpawnActor<ADialogueManager>(DialogueManagerBP);

		if (DialogueManager)
		{
			DialogueManager->GameManager = this;
			DialogueManager->MainPlayer = MainPlayer;

			DialogueManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (ItemManagerBP)
	{
		ItemManager = GetWorld()->SpawnActor<AItemManager>(ItemManagerBP);

		if (ItemManager)
		{
			ItemManager->GameManager = this;
			ItemManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (NpcManagerBP)
	{
		NpcManager = GetWorld()->SpawnActor<ANpcManager>(NpcManagerBP);

		if (NpcManager)
		{
			NpcManager->GameManager = this;
			NpcManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			NpcManager->SetAllNpcData();

		}
	}

	if (QuestManagerBP)
	{
		QuestManager = GetWorld()->SpawnActor<AQuestManager>(QuestManagerBP);

		if (QuestManager)
		{
			QuestManager->GameManager = this;
			QuestManager->MainPlayer = MainPlayer;
			QuestManager->PlayerQuest = MainPlayer->PlayerQuest;
			QuestManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			QuestManager->SetAllQuestData();
		}
	}

	if (LevelManagerBP)
	{
		LevelManager = GetWorld()->SpawnActor<ALevelManager>(LevelManagerBP);

		if (LevelManager)
		{
			LevelManager->GameManager = this;
			LevelManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (MainPlayer)
	{
		MainPlayer->GameManager = this;
	}

#pragma endregion

}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

