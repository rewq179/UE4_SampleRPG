// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/QuestManager.h"
#include "PlayerQuest.generated.h"

/**
 * 플레이어가 보유한 퀘스트에 관한 함수와 변수들을 모아놓음.
*/

UCLASS()
class SAMPLERPG_API APlayerQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerQuest();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|ManagerClass")
	class AQuestManager* QuestManager;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Components")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Properties")
	class ANpcController* InteractNPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Quest")
	TMap<int32, FQuestTable> Quests;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Quest")
	TArray<int32> QuestKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Quest")
	TArray<int32> QuestTepList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Quest")
	FQuestTable RecentQuest;
	
public:	

	// Add & Remove Quest //

	void AddQuest();
	void RemoveQuest();
	void SetQuestKey();


	void ClearQuest(int32 QuestID);

	
	UFUNCTION(BlueprintCallable)
	void SetQuestTepList(bool bIsClearTep);

	UFUNCTION(BlueprintCallable)
	bool IsClearQuestExist();

	UFUNCTION(BlueprintCallable)
	bool CanGetQuest();

	UFUNCTION(BlueprintCallable)
	FQuestTable GetQuestData(int32 QuestID);

	void IncreaseCount(EQuestType QuestType, int32 TargetID, int32 Count);

	// Save & Load GameData //

	void SaveQuestData(class USaveGameManager* SaveGameInstance);
	void LoadQuestData(class USaveGameManager* LoadGameInstance);

	
	
};
