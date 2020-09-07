// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "QuestManager.generated.h"

UCLASS()
class SAMPLERPG_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|ManagerClass")
	class ANpcManager* NpcManager;
	
	// Properties //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|Properties")
	class AMainPlayer* MainPlayer;
	
	// DataTable //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestManager|Quests")
	TMap<int32, FQuestTable> QuestMap;

protected:

public:	
	UFUNCTION(BlueprintCallable)
	FQuestTable GetQuestData(int32 QuestID);

	void SetQuestData(int32 QuestID);
	void SetQuestDataAll();

	// Hud //

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetRewardIcon(int32 ItemID);

	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FQuestTable Quest);

	UFUNCTION(BlueprintCallable)
	void ClearQuest(FQuestTable Quest);

	// Quest Symbol(!, ?) Check //

	bool IsExclamationSymbol(TArray<int32> QuestID); 
	bool IsPrerequisiteMeet(int32 QuestID); // 선행 조건을 만족했는지?
};
