// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/ItemManager.h"
#include "Manager/GameManager.h"
#include "Engine/World.h"
#include "Item/Item.h"


// Sets default values
AItemManager::AItemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

FItemTable AItemManager::GetItemTableData(int32 ItemID)
{
	AItem* Item = ItemMap[ItemID].GetDefaultObject();

	Item->ItemID = ItemID;
	Item->SetItemData();

	SetItemData(Item, ItemID);

	return Item->ItemData;
}

void  AItemManager::SetItemData(AItem* Item, int32 ItemID)
{
	FItemTable* ItemTableData = GameManager->DataTableManager->GetItemData(ItemID);
	
	if (ItemTableData)
	{
		Item->ItemData.Name = (*ItemTableData).Name;

		Item->ItemData.ItemID = (*ItemTableData).ItemID;
		Item->ItemData.ItemClass = (*ItemTableData).ItemClass;
		Item->ItemData.ItemType = (*ItemTableData).ItemType;
		Item->ItemData.Name = (*ItemTableData).Name;
		Item->ItemData.Description = (*ItemTableData).Description;
		Item->ItemData.bIsDroppable = (*ItemTableData).bIsDroppable;
		Item->ItemData.bIsSellable = (*ItemTableData).bIsSellable;
		Item->ItemData.BuyPrice = (*ItemTableData).BuyPrice;
		Item->ItemData.SellPrice = (*ItemTableData).SellPrice;
		Item->ItemData.MaxCount = (*ItemTableData).MaxCount;
		Item->ItemData.Damage = (*ItemTableData).Damage;
		Item->ItemData.Deffence = (*ItemTableData).Deffence;
		Item->ItemData.Strength = (*ItemTableData).Strength;
		Item->ItemData.Dexterity = (*ItemTableData).Dexterity;
		Item->ItemData.Intelligence = (*ItemTableData).Intelligence;
		Item->ItemData.Mesh = (*ItemTableData).Mesh;

		Item->ItemData.Mesh = (*ItemTableData).Mesh;

		if (Item->ItemData.ItemClass == EItemClass::EIC_Equip && (*ItemTableData).EquipMesh != nullptr)
		{
			Item->ItemData.EquipMesh = (*ItemTableData).EquipMesh;
		}

		Item->ItemData.Icon = (*ItemTableData).Icon;
	}
}

AItem* AItemManager::CreateItemActor(int32 ItemID, int32 Count)
{
	AItem* Item = GetWorld()->SpawnActor<AItem>(ItemMap[ItemID]);

	Item->Count = Count;
	Item->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	SetItemData(Item, ItemID);

	return Item;
}

