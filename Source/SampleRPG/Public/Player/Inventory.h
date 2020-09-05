// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Item.h"
#include "Inventory.generated.h"

UCLASS()
class SAMPLERPG_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|ManagerClass")
	class AItemManager* ItemManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Components")
	class AMainPlayer* MainPlayer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Inventory")
	int32 Gold;
	FORCEINLINE void AddGold(int32 Value) { Gold += Value; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Inventory")
	TArray<class AItem*> Spaces;
	   
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Equipment")
	TArray<class AItem*> Equipments;

public:	
	// 인벤토리에 아이템 추가 및 제거 //

	UFUNCTION(BlueprintCallable)
	void BuyItem(class AItem* Item);

	UFUNCTION(BlueprintCallable)
	void SellItem(class AItem* Item, int32 InputCount);

	void AddItem(class AItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(class AItem* Item, int32 Count);

	int32 GetInventoryIndex(class AItem* Item);
	void DevideItemCount(class AItem* Item, int32 TotalCount);

	// 아이템 사용 및 장착 //

	UFUNCTION(BlueprintCallable)
	void UseItem(class AItem* Item, int32 SlotIndex);

	void ConsumeItem(class AItem* Item, int32 SlotIndex);

	void EquipItem(class AItem* NewItem, int32 InvIndex);
	UFUNCTION(BlueprintCallable)
	void UnEquipItem(int32 SlotIndex);

	void WasStatusChangedByEquip(class AItem* Item, bool IsEquip);

	// HUD 갱신 //

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventorySlot();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquipmentSlot();

	// 상점창 기능 //

	UFUNCTION(BlueprintCallable)
	bool IsEnoughGold(int32 Price, int32 Count);

	UFUNCTION(BlueprintCallable)
	int32 GetItemMaxCount(FItemTable ItemTableValue, int32 ItemCount, int32 InputCount, bool IsBuyTep);
	UFUNCTION(BlueprintCallable)
	int32 GetItemPrice(FItemTable ItemTableValue, int32 Count, bool IsBuyTep);

	// 세이브 로드 //

	void SaveInventoryData(class USaveGameManager* SaveGameInstance);
	int32 GetItemKey(TMap<int32, int32> Map, int32 ID);
	void LoadInventoryData(class USaveGameManager* LoadGameInstance);

};
