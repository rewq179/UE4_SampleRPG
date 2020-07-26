// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "DialogueManager.generated.h"

USTRUCT(BlueprintType)
struct FDialogueTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
	int32 DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
	FString Description;
};

UCLASS()
class SAMPLERPG_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue|Properties")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue|Properties")
	class ANpcController* InteractNPC;

	class UDataTable* DialogueTable;
	FDialogueTable* DialogueTableRow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue|DataTable")
	FDialogueTable Dialogue;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue|DataTable")
	int32 DialogueID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue|Interact")
	int32 InteractType;
	
	void SetActiveDialouge(class AMainPlayer* Player, class ANpcController* Npc);
	void SetDialogueText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerBP(bool IsNull);

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDialogue();

	void UpdateInteractTypeBox();

	
};
