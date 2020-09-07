// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/DialogueManager.h"
#include "Manager/GameManager.h"
#include "Npc/NpcController.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADialogueManager::ADialogueManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADialogueManager::SetNPC(ANpcController* Npc)
{
	if (InteractNPC != Npc)
	{
		InteractNPC = Npc;
	}

	if (Npc)
	{
		SetConversationalDialogue();
	}

	SetActiveDialogue();
}

void ADialogueManager::SetConversationalDialogue()
{
	int32 DialogueID = FMath::RandRange(InteractNPC->DialogueID[0], InteractNPC->DialogueID[InteractNPC->DialogueID.Num()-1]);

	Dialogue = GetDialogue(DialogueID);
}

FDialogueTable ADialogueManager::GetDialogue(int32 DialogueID)
{
	FDialogueTable* DialogueTableData = GameManager->DataTableManager->GetDialogueTableData(DialogueID);

	FDialogueTable Dialogue;

	if (DialogueTableData)
	{
		Dialogue.DialogueID = (*DialogueTableData).DialogueID;
		Dialogue.DialogueText = (*DialogueTableData).DialogueText;
	}

	return Dialogue;
}
