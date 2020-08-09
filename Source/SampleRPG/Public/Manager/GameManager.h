// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"
#include "Manager/SaveGameManager.h"
#include "Manager/LevelManager.h"
#include "Manager/DialogueManager.h"
#include "Manager/ItemManager.h"
#include "Manager/NpcManager.h"
#include "Manager/QuestManager.h"

#include "GameManager.generated.h"

UCLASS()
class SAMPLERPG_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameManager|Properties")
	class AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Properties")
	TSubclassOf<class ADataTableManager> DataTableManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Properties")
	class ADataTableManager* DataTableManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Properties")
	TSubclassOf<class ADialogueManager> DialogueManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Properties")
	class ADialogueManager* DialogueManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Properties")
	TSubclassOf<class AItemManager> ItemManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Properties")
	class AItemManager* ItemManager;

	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Properties")
	TSubclassOf<class ANpcManager> NpcManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Properties")
	class ANpcManager* NpcManager;

	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Properties")
	TSubclassOf<class AQuestManager> QuestManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Properties")
	class AQuestManager* QuestManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Properties")
	TSubclassOf<class ALevelManager> LevelManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Properties")
	class ALevelManager* LevelManager;

	void GetDataName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
