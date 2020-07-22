// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "MainPlayer.h"
#include "Inventory.generated.h"

UCLASS()
class SAMPLERPG_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	class AMainPlayer* MainPlayer;
	
	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|Properties")
	TSubclassOf<class AItemStorage> ItemStorageBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|SaveData")
	class AItemStorage* ItemStorage;
	
	// 보유 골드
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	int32 Gold;

	FORCEINLINE void AddGold(int32 Value) { Gold += Value; }


	// 아이템 획득/제거
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	TArray<AItem*> Spaces;

	void AddItem(AItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(AItem* Item);

	int32 GetInventoryIndex(AItem* Item);
	void DevideItemCount(AItem* Item, int32 TotalCount);


	// 아이템 사용
	UFUNCTION(BlueprintCallable)
	void UseItem(AItem* Item, int32 SlotIndex);
	void ConsumeItem(AItem* Item, int32 SlotIndex);


	// 아이템 장착 및 해제
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Equipment")
	TArray<AItem*> Equipments;

	void EquipItem(AItem* NewItem, int32 InvIndex);
	UFUNCTION(BlueprintCallable)
	void UnEquipItem(int32 SlotIndex);

	int32 GetEquipmentIndex(AItem* Item);

	void WasStatusChangedByEquip(AItem* Item, bool IsEquip);
	

	// 슬롯 아이콘 갱신
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventorySlot();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquipmentSlot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	AItem* CreateItemActor(int32 ItemID);
	
	void SaveInventoryData(class URPGSaveGame* SaveGameInstance);
	void LoadInventoryData(class URPGSaveGame* LoadGameInstance);

	int32 GetItemKey(TMap<int32, int32> Map, int32 ID);
};
