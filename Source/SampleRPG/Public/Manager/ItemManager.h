// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Item.h"
#include "Manager/DataTableManager.h"

#include "ItemManager.generated.h"

UCLASS()
class SAMPLERPG_API AItemManager : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AItemManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemManager|ItemData")
	TMap<int32, TSubclassOf<class AItem>> ItemMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemManager|RewardData")
		TMap<int32, FRewardTable> RewardMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	FItemTable GetItemData(int32 ItemID);

	void SetItemData(class AItem* Item, int32 ItemID);

	UFUNCTION(BlueprintCallable)
	class AItem* CreateItemActor(int32 ItemID, int32 Count);

	void SpawnItemActor(int32 ITemID, int32 Count, FVector Location);

	UFUNCTION(BlueprintCallable)
	FRewardTable GetRewardData(int32 RewardID);

	void SetRewardDataAll();
	void SetRewardData(int32 RewardID);

	FRewardBox RewardBox(int32 ID, int32 Count, float Percent);

	void GetMonsterItem(int32 ProductID, int32 RewardID, FVector Location);
	bool GetItemInRewardBox(int32 RewardID, FVector Location);

};
