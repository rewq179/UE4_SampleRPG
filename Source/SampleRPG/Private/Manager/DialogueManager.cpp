// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/DialogueManager.h"
#include "Npc/NpcController.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADialogueManager::ADialogueManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/DialogueTable.DialogueTable'"));

	if (DataTable.Succeeded())
	{
		DialogueTable = DataTable.Object;
	}
}

// Called when the game starts or when spawned
void ADialogueManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADialogueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogueManager::SetActiveDialouge(class AMainPlayer* Player, class ANpcController* Npc)
{
	if (InteractNPC != Npc)
	{
		InteractNPC = Npc;
	}

	if (MainPlayer != Player)
	{
		MainPlayer = Player;
	}

	if (MainPlayer && Npc)
	{
		SetPlayerBP(false);

		SetDialogueText();
		UpdateInteractTypeBox();

		SetActiveDialogue();
	}

	else
	{
		SetPlayerBP(true);
	}
}

void ADialogueManager::SetDialogueText()
{
	DialogueID = FMath::RandRange(InteractNPC->DialogueID[0], InteractNPC->DialogueID[InteractNPC->DialogueID.Num()-1]);

	if (DialogueTable)
	{
		DialogueTableRow = DialogueTable->FindRow<FDialogueTable>(FName(*(FString::FormatAsNumber(DialogueID))), FString(""));

		if (DialogueTableRow)
		{
			Dialogue.DialogueID = (*DialogueTableRow).DialogueID;
			Dialogue.DialogueText = (*DialogueTableRow).DialogueText;
		}
	}
}

void ADialogueManager::UpdateInteractTypeBox()
{
	if (InteractNPC->Npc.bHasQuest && InteractNPC->Npc.bHasItem)
	{
		InteractType = 3;
	}

	else if (InteractNPC->Npc.bHasQuest)
	{
		InteractType = 2;
	}

	else if (InteractNPC->Npc.bHasItem)
	{
		InteractType = 1;
	}

	else
	{
		InteractType = 0;
	}
	
}