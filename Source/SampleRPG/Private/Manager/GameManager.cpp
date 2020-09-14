// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"

#include "Player/MainPlayer.h"

#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsLobbyLevel = false;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (MainPlayer)
	{
		MainPlayer->GameManager = this;
	}

	InitComponents();

	FTimerHandle Timer;
		
	GetWorldTimerManager().SetTimer(Timer, this, &AGameManager::DelayFunction, 2.f, false);
}

void AGameManager::InitComponents()
{
	if (CombatManagerBP)
	{
		CombatManager = GetWorld()->SpawnActor<ACombatManager>(CombatManagerBP);

		if (CombatManager)
		{
			CombatManager->GameManager = this;
			CombatManager->MainPlayer = MainPlayer;
			CombatManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

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

	if (DungeonManagerBP)
	{
		DungeonManager = GetWorld()->SpawnActor<ADungeonManager>(DungeonManagerBP);

		if (DungeonManager)
		{
			DungeonManager->GameManager = this;
			DungeonManager->MainPlayer = MainPlayer;
			DungeonManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			DungeonManager->SetDungeonDataAll();
		}
	}


	if (ItemManagerBP)
	{
		ItemManager = GetWorld()->SpawnActor<AItemManager>(ItemManagerBP);

		if (ItemManager)
		{
			ItemManager->GameManager = this;
			ItemManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			ItemManager->SetRewardDataAll();
		}
	}

	if (NpcManagerBP)
	{
		NpcManager = GetWorld()->SpawnActor<ANpcManager>(NpcManagerBP);

		if (NpcManager)
		{
			NpcManager->GameManager = this;
			NpcManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			NpcManager->SetNpcDataAll();

		}
	}

	if (QuestManagerBP)
	{
		QuestManager = GetWorld()->SpawnActor<AQuestManager>(QuestManagerBP);

		if (QuestManager)
		{
			QuestManager->GameManager = this;
			QuestManager->MainPlayer = MainPlayer;
			QuestManager->NpcManager = NpcManager;
			QuestManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			QuestManager->SetQuestDataAll();
		}
	}

	if (PatternManagerBP)
	{
		PatternManager = GetWorld()->SpawnActor<APatternManager>(PatternManagerBP);

		if (PatternManager)
		{
			PatternManager->GameManager = this;
			PatternManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (SkillManagerBP)
	{
		SkillManager = GetWorld()->SpawnActor<ASkillManager>(SkillManagerBP);

		if (SkillManager)
		{
			SkillManager->GameManager = this;
			SkillManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			SkillManager->SetSkillDataAll();

			CombatManager->SkillManager = SkillManager;
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
}

void AGameManager::DelayFunction()
{
	NpcManager->CheckQuestSymbolAllNPC();
}

