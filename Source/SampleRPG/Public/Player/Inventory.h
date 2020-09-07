// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Item.h"
#include "Inventory.generated.h"

/**
 * 플레이어의 인벤토리에서 사용될 정보와 함수들을 모아놓음.
*/

UCLASS()
class SAMPLERPG_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|ManagerClass")
	class AItemManager* ItemManager;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Components")
	class AMainPlayer* MainPlayer;
	
	// Inventory //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Inventory")
	int32 Gold;
	FORCEINLINE void AddGold(int32 Value) { Gold += Value; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Inventory")
	TArray<class AItem*> Spaces; 

	// Equipment //
	   
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Equipment")
	TArray<class AItem*> Equipments;

public:	

	// Add & Remove //

	void AddItem(class AItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(class AItem* Item, int32 Count);

	int32 GetInventoryIndex(class AItem* Item);
	void DevideItemCount(class AItem* Item, int32 TotalCount);

	// HUD : Buy And Sell //

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventorySlot();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquipmentSlot();

	UFUNCTION(BlueprintCallable)
	void BuyItem(class AItem* Item);

	UFUNCTION(BlueprintCallable)
	void SellItem(class AItem* Item, int32 InputCount);

	UFUNCTION(BlueprintCallable)
	bool IsEnoughGold(int32 Price, int32 Count);

	UFUNCTION(BlueprintCallable)
	int32 GetItemMaxCount(FItemTable ItemTableValue, int32 ItemCount, int32 InputCount, bool IsBuyTep);

	UFUNCTION(BlueprintCallable)
	int32 GetItemPrice(FItemTable ItemTableValue, int32 Count, bool IsBuyTep);

	// HUD : Click Item //

	UFUNCTION(BlueprintCallable)
	void UseItem(class AItem* Item, int32 SlotIndex);

	void ConsumeItem(class AItem* Item, int32 SlotIndex);

	void EquipItem(class AItem* NewItem, int32 InvIndex);
	UFUNCTION(BlueprintCallable)
	void UnEquipItem(int32 SlotIndex);

	void WasStatusChangedByEquip(class AItem* Item, bool IsEquip);
		
	// Save & Load GameData //

	void SaveInventoryData(class USaveGameManager* SaveGameInstance);
	int32 GetItemKey(TMap<int32, int32> Map, int32 ID);
	
	void LoadInventoryData(class USaveGameManager* LoadGameInstance);

};
