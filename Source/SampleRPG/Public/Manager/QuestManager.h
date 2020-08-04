// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item/Item.h"
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
	int32 QuestSubID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	EQuestClass QuestClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	EQuestType QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
	FText Goal;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|Properties")
	class AGameManager* GameManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestManager|Properties")
	class AMainPlayer* MainPlayer;

	class UDataTable* QuestTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestManager|QuestTable")
	TMap<int32, FQuestTable> QuestMap;

	void SetQuestData(int32 QuestID);

	UFUNCTION(BlueprintCallable)
	FQuestTable GetQuestData(int32 QuestID);

	FORCEINLINE int32 GetPreQuestID(int32 QuestID) { return QuestMap[QuestID].PreQuest; }


	UFUNCTION(BlueprintCallable)
	UTexture2D* GetRewardIcon(int32 ItemID);

	UFUNCTION(BlueprintCallable)
	void AcceptQuest();

	UFUNCTION(BlueprintCallable)
	void ClearQuest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
