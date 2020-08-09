// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/QuestManager.h"
#include "PlayerQuest.generated.h"

UCLASS()
class SAMPLERPG_API APlayerQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerQuest();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Properties")
	class AQuestManager* QuestManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerQuest|Properties")
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

	void AddQuest();
	void RemoveQuest();
	void ClearQuest(int32 QuestID);

	void SetQuestKey();
	
	UFUNCTION(BlueprintCallable)
	void SetQuestTepList(bool bIsClearTep);

	UFUNCTION(BlueprintCallable)
	bool IsExistClearQuest();

	UFUNCTION(BlueprintCallable)
	bool IsExistQuest();

	UFUNCTION(BlueprintCallable)
	FQuestTable GetQuestData(int32 QuestID);

	void IncreaseCount(EQuestType QuestType, int32 TargetID, int32 Count);

	void SaveQuestData(class USaveGameManager* SaveGameInstance);
	void LoadQuestData(class USaveGameManager* LoadGameInstance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
