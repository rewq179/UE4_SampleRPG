// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Manager/DataTableManager.h"
#include "DialogueManager.generated.h"

/**
 * Dailogue Hud와 대화하기 위한 Mamanger Class이다.
*/

UCLASS()
class SAMPLERPG_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|ManagerClass")
	class AGameManager* GameManager;

	// Data Table //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|DataTable")
	FDialogueTable Dialogue;

	// Interact //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|Interact")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|Interact")
	class ANpcController* InteractNPC;
		
public:	
	void SetNPC(class ANpcController* Npc);

	FDialogueTable GetDialogue(int32 DialogueID);
	void SetConversationalDialogue();
	
	// HUD //

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDialogue();
};
