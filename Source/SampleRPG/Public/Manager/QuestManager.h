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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|Properties")
	class AGameManager* GameManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|Properties")
	class AMainPlayer* MainPlayer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|Properties")
	class APlayerQuest* PlayerQuest;

	class UDataTable* QuestTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestManager|QuestTable")
	TMap<int32, FQuestTable> QuestMap;

	void SetAllQuestData();
	void SetQuestData(int32 QuestID);

	bool IsPrerequisiteMeet(int32 QuestID);

	UFUNCTION(BlueprintCallable)
	FQuestTable GetQuestData(int32 QuestID);

	FORCEINLINE int32 GetPreQuestID(int32 QuestID) { return QuestMap[QuestID].PreQuest; }


	UFUNCTION(BlueprintCallable)
	UTexture2D* GetRewardIcon(int32 ItemID);

	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FQuestTable Quest);

	UFUNCTION(BlueprintCallable)
	void ClearQuest(FQuestTable Quest);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	void SetSymbol(ESymbolType SymbolType, int32 QuestID);
	bool IsExclamationSymbol(TArray<int32> QuestID);
};
