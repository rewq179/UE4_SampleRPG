// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/ItemManager.h"
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

FItemTable AItemManager::GetItemTableValue(int32 ItemID)
{
	AItem* Item = ItemMap[ItemID].GetDefaultObject();

	Item->ItemID = ItemID;
	Item->SetItemData();

	return Item->ItemTableValue;
}

AItem* AItemManager::CreateItemActor(int32 ItemID, int32 Count)
{
	AItem* Item = GetWorld()->SpawnActor<AItem>(ItemMap[ItemID]);

	Item->Count = Count;
	Item->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	return Item;
}

