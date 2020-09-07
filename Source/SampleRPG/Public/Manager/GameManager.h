// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/CombatManager.h"
#include "Manager/DataTableManager.h"
#include "Manager/SaveGameManager.h"
#include "Manager/LevelManager.h"
#include "Manager/DialogueManager.h"
#include "Manager/ItemManager.h"
#include "Manager/NpcManager.h"
#include "Manager/QuestManager.h"
#include "Manager/PatternManager.h"
#include "Manager/SkillManager.h"

#include "GameManager.generated.h"

/**
 * Manager BP와 Class들을 모아놓음.
*/

UCLASS()
class SAMPLERPG_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameManager|Properties")
	class AMainPlayer* MainPlayer;

#pragma region Components
	
	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
	TSubclassOf<class ACombatManager> CombatManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class ACombatManager* CombatManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|PComponents")
	TSubclassOf<class ADataTableManager> DataTableManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class ADataTableManager* DataTableManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
	TSubclassOf<class ADialogueManager> DialogueManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class ADialogueManager* DialogueManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
	TSubclassOf<class AItemManager> ItemManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class AItemManager* ItemManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
	TSubclassOf<class ANpcManager> NpcManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class ANpcManager* NpcManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
	TSubclassOf<class AQuestManager> QuestManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class AQuestManager* QuestManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
	TSubclassOf<class APatternManager> PatternManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class APatternManager* PatternManager;


	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
		TSubclassOf<class ASkillManager> SkillManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
		class ASkillManager* SkillManager;

	UPROPERTY(EditDefaultsOnly, Category = "GameManager|Components")
	TSubclassOf<class ALevelManager> LevelManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameManager|Components")
	class ALevelManager* LevelManager;

#pragma endregion


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void InitComponents();

	void DelayFunction();
	
	
};
