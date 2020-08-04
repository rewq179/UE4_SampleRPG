// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "DataTableManager.generated.h"

USTRUCT(BlueprintType)
struct FNpcTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		int32 NpcID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		bool bHasItem = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		bool bHasQuest = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		FString DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		FString QuestID;
};

UCLASS()
class SAMPLERPG_API ADataTableManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataTableManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTableManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataTableManager|DataTable")
	class AMainPlayer* MainPlayer;

	class UDataTable* NpcTable;
	FNpcTable* NpcData;

	FNpcTable* GetNpcData(int32 NpcID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
