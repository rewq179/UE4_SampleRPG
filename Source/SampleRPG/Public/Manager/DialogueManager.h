// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Manager/DataTableManager.h"
#include "DialogueManager.generated.h"

UCLASS()
class SAMPLERPG_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|Properties")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|Properties")
	class ANpcController* InteractNPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|DataTable")
	FDialogueTable Dialogue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|DataTable")
	int32 DialogueID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueManager|Interact")
	int32 InteractType;
	
	void SetActiveDialouge(bool IsActive, class ANpcController* Npc);
	void SetDialogueText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDialogue(bool IsActive);

	void UpdateInteractTypeBox();
};
