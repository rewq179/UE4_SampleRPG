// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "QuestManager.generated.h"


UENUM(BlueprintType)
enum class EQuestClass : uint8
{
	EQC_Main UMETA(DisplayName = "Main"),
	EQC_Sub UMETA(DisplayName = "Sub"),
	EQC_Repeat UMETA(DisplayName = "Repeat"),

	EQC_MAX
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	EQT_Kill UMETA(DisplayName = "Kill"),
	EQT_Collect UMETA(DisplayName = "Collect"),
	EQT_Use UMETA(DisplayName = "Use"),
	EQT_Communication UMETA(DisplayName = "Communication"),
	
	EQT_MAX
};

// 헤더파일 : #include "Engine/DataTable.h"

USTRUCT(BlueprintType)
struct FQuestTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 QuestID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	EQuestClass QuestClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	EQuestType QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	FString Goal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	FString PreDialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	FString PostDialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 PreQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	bool bCanClear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	bool bIsClear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 NpcID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 PreReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 PreRewardCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 PostReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	int32 PostRewardCount;
};

UCLASS()
class SAMPLERPG_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

	class UDataTable* QuestTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest|QuestTable")
	TMap<int32, FQuestTable> QuestMap;

	void SetQuestData(int32 QuestID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
