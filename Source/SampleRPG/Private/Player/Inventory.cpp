// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Inventory.h"
#include "Player/MainPlayer.h"

#include "Item/Item.h"
#include "Manager/ItemManager.h"
#include "Manager/SaveGameManager.h"
#include "Manager/QuestManager.h"
#include "Player/PlayerQuest.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Equipments.Init(nullptr, 10);
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();

	AddGold(10000);
}

void AInventory::BuyItem(AItem* Item)
{
	AddGold(-Item->ItemData.BuyPrice * Item->Count);

	AddItem(Item);
}


void AInventory::SellItem(AItem* Item, int32 InputCount)
{
	AddGold(Item->ItemData.SellPrice * InputCount);

	RemoveItem(Item, InputCount);
}


void AInventory::AddItem(AItem* Item)
{
	/*
		원리 :  인벤토리를 훑어서 아이템이 존재하는지 확인한다.

		if) O : Item의 Count를 증가한다. Count가 Max를 넘어가면 새로운 공간에 Item을 배치한다.
		if) X : 새로운 공간에 해당 Item을 배치한다.
	*/
	int32 InvIndex = GetInventoryIndex(Item);
	int32 TotalCount = Item->Count;
	int32 MaxCount = Item->ItemData.MaxCount;

	Item->SetItemState(EItemState::EIS_Inv);
	Item->ItemOwner = MainPlayer;
	Item->SetActorLocation(FVector(0.f));

	MainPlayer->PlayerQuest->IncreaseCount(EQuestType::EQT_Collect, Item->ItemID, Item->Count);

	if (InvIndex != -1) // 인벤토리에 아이템이 존재한다
	{
		TotalCount += Spaces[InvIndex]->Count;

		if (TotalCount > MaxCount)
		{
			Spaces[InvIndex]->Count = MaxCount;
			TotalCount -= MaxCount;
			
			DevideItemCount(Item, TotalCount);
		}

		else
		{
			Spaces[InvIndex]->Count += Item->Count;
		}
	}

	else // 인벤토리에 아이템이 없다
	{
		if (Item->Count > MaxCount)
		{
			Item->Count = MaxCount;
			TotalCount -= MaxCount;
			Spaces.Add(Item);

			DevideItemCount(Item, TotalCount);
		}

		else
		{
			Item->SetActorLocation(FVector(255.f));

			Spaces.Add(Item);
		}
	}


	if (Item->ItemData.ItemClass == EItemClass::EIC_Equip)
	{
		Item->IgnoreStaticMesh();
		Item->SetCombatCollisionEnabled(false);
	}
	
	UpdateInventorySlot();
}

int32 AInventory::GetInventoryIndex(AItem* Item)
{
	for (int32 InvIndex = 0; InvIndex != Spaces.Num(); InvIndex++) // 해당 아이템이 있나?
	{
		if (Spaces[InvIndex]->ItemID == Item->ItemID)
		{
			if (Spaces[InvIndex]->Count < Item->ItemData.MaxCount)
			{
				return InvIndex;
			}
		}
	}

	return -1;
}

void AInventory::DevideItemCount(AItem* Item, int32 TotalCount)
{
	while (TotalCount > 0)
	{
		AItem* DevideItem = ItemManager->CreateItemActor(Item->ItemID, 1);
		DevideItem->SetActorLocation(FVector(255.f));

		if (TotalCount > Item->ItemData.MaxCount)
		{
			DevideItem->Count = Item->ItemData.MaxCount;
		}

		else
		{
			DevideItem->Count = TotalCount;
		}

		Spaces.Add(DevideItem);
		TotalCount -= Item->ItemData.MaxCount;
	}
}

void AInventory::RemoveItem(AItem* Item, int32 Count)
{
	int32 SlotIndex = Spaces.IndexOfByKey(Item);

	int32 TotalCount = Spaces[SlotIndex]->Count - Count;

	if (TotalCount > 0)
	{
		Spaces[SlotIndex]->Count -= Count;
	}

	else
	{
		Spaces.RemoveAt(SlotIndex);
	}
	
	UpdateInventorySlot();
}

void AInventory::UseItem(AItem* Item, int32 SlotIndex)
{
	if (Item->Count > 0)
	{
		if (Item->ItemData.ItemClass == EItemClass::EIC_Equip)
		{
			EquipItem(Item, SlotIndex);

			MainPlayer->PlayerQuest->IncreaseCount(EQuestType::EQT_Equip, Item->ItemID, 1);
		}

		else if (Item->ItemData.ItemClass == EItemClass::EIC_Consume)
		{
			ConsumeItem(Item, SlotIndex);

			MainPlayer->PlayerQuest->IncreaseCount(EQuestType::EQT_Use, Item->ItemID, 1);
		}
	}
}

void AInventory::ConsumeItem(AItem* Item, int32 SlotIndex)
{
	Spaces[SlotIndex]->Count--;

	MainPlayer->PlayerStatus->AdjustHP(Item->ItemData.Damage, false);

	if (Item->UseSound)
	{
		UGameplayStatics::PlaySound2D(this, Item->UseSound);
	}
}

void AInventory::EquipItem(AItem* NewItem, int32 InvIndex)
{
	/*
		0:무기, 1:방패, 2:헬멧, 3:갑옷, 4:어깨방어구, 5:장갑, 6:하의, 7:신발, 8:악세 1, 9:악세 2
	*/
	Spaces.RemoveAt(InvIndex);

	int32 EquipIndex = GetEquipmentIndex(NewItem);

	// 헤더파일 : #include "Engine/SkeletalMeshSocket.h"

	if (EquipIndex == 0) // 무기
	{
		
		const USkeletalMeshSocket* RightHandSocket = MainPlayer->GetMesh()->GetSocketByName("RightHandSocket");

		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(NewItem, MainPlayer->GetMesh());
		}

		MainPlayer->bIsEquip = true;
	}

	else if (EquipIndex == 1) // 방패
	{
		const USkeletalMeshSocket* LeftHandSocket = MainPlayer->GetMesh()->GetSocketByName("LeftHandSocket");

		if (LeftHandSocket)
		{
			LeftHandSocket->AttachActor(NewItem, MainPlayer->GetMesh());
		}
	}

	else if (EquipIndex == 8) // 반지
	{
		if (Equipments[8] && !Equipments[9])
		{
			EquipIndex++;
		}
	}

	if (Equipments[EquipIndex])
	{
		UnEquipItem(EquipIndex);
	}

	Equipments[EquipIndex] = NewItem;
	WasStatusChangedByEquip(NewItem, true);

	if (Equipments[EquipIndex]->UseSound)
	{
		UGameplayStatics::PlaySound2D(this, Equipments[EquipIndex]->UseSound);
	}
}

void AInventory::UnEquipItem(int32 SlotIndex)
{
	WasStatusChangedByEquip(Equipments[SlotIndex], false);
	
	if (SlotIndex == 0)
	{
		MainPlayer->bIsEquip = false;
	}

	AddItem(Equipments[SlotIndex]);
	Equipments[SlotIndex] = nullptr;
}

int32 AInventory::GetEquipmentIndex(AItem* Item)
{
	switch (Item->ItemData.ItemType)
	{
	case EItemType::EIT_Weapon:
		return 0;

	case EItemType::EIT_Shield:
		return 1;

	case EItemType::EIT_Helmet:
		return 2;

	case EItemType::EIT_Chest:
		return 3;

	case EItemType::EIT_Shoulder:
		return 4;

	case EItemType::EIT_Glove:
		return 5;

	case EItemType::EIT_Pants:
		return 6;

	case EItemType::EIT_Boots:
		return 7;

	case EItemType::EIT_Ring:
		return 8;

	default:
		return -1;
	}
}

void AInventory::WasStatusChangedByEquip(AItem* Item, bool IsEquip)
{
	if (IsEquip)
	{
		MainPlayer->PlayerStatus->IncDamage += Item->ItemData.Damage;
		MainPlayer->PlayerStatus->IncDeffence += Item->ItemData.Deffence;
		MainPlayer->PlayerStatus->IncStrength += Item->ItemData.Strength;
		MainPlayer->PlayerStatus->IncDexterity += Item->ItemData.Dexterity;
		MainPlayer->PlayerStatus->IncIntelligence += Item->ItemData.Intelligence;
	}
	
	else
	{
		MainPlayer->PlayerStatus->IncDamage -= Item->ItemData.Damage;
		MainPlayer->PlayerStatus->IncDeffence -= Item->ItemData.Deffence;
		MainPlayer->PlayerStatus->IncStrength -= Item->ItemData.Strength;
		MainPlayer->PlayerStatus->IncDexterity -= Item->ItemData.Dexterity;
		MainPlayer->PlayerStatus->IncIntelligence -= Item->ItemData.Intelligence;
	}
}

void AInventory::SaveInventoryData(USaveGameManager* SaveGameInstance)
{
	for (auto Item : Spaces)
	{
		if (SaveGameInstance->InventoryItemMap.Contains(Item->ItemID))
		{
			int32 Key = GetItemKey(SaveGameInstance->InventoryItemMap, Item->ItemID);
			int32 Value = SaveGameInstance->InventoryItemMap[Key];

			SaveGameInstance->InventoryItemMap.Emplace(Item->ItemID, Value + Item->Count);
		}

		else
		{
			SaveGameInstance->InventoryItemMap.Add(Item->ItemID, Item->Count);
		}
	}

	for (int32 SlotIndex = 0; SlotIndex < 10; SlotIndex++)
	{
		if (Equipments[SlotIndex])
		{
			SaveGameInstance->EquipmentItem[SlotIndex] = Equipments[SlotIndex]->ItemID;
		}

		else
		{
			SaveGameInstance->EquipmentItem[SlotIndex] = -1;
		}
	}
}
	
int32 AInventory::GetItemKey(TMap<int32, int32> Map, int32 ID)
{
	int32 Key = -1;

	for (auto& Item : Map)
	{
		if (Item.Key == ID)
		{
			Key = Item.Key;
		}
	}

	return Key;
}


void AInventory::LoadInventoryData(USaveGameManager* LoadGameInstance)
{
	if (ItemManager)
	{
		Spaces.Empty();

		for (auto InvItem : LoadGameInstance->InventoryItemMap)
		{
			AItem* Item = ItemManager->CreateItemActor(InvItem.Key, InvItem.Value);

			AddItem(Item);
		}

		for (int32 SlotIndex = 0; SlotIndex < 10; SlotIndex++)
		{
			Equipments[SlotIndex] = nullptr;

			if (LoadGameInstance->EquipmentItem[SlotIndex] >= 0)
			{
				AItem* Item = ItemManager->CreateItemActor(LoadGameInstance->EquipmentItem[SlotIndex], 1);

				AddItem(Item);
				EquipItem(Item, Spaces.Num() - 1);
			}
		}
	}
}

bool AInventory::IsEnoughGold(int32 Price, int32 Count)
{
	if (Price * Count <= Gold && Count > 0)
	{
		return true;
	}

	return false;
}

int32 AInventory::GetItemMaxCount(FItemTable ItemData, int32 ItemCount, int32 InputCount, bool IsBuyTep)
{
	if (IsBuyTep) // 구매 탭
	{
		if (InputCount > ItemData.MaxCount) // 구입 개수가 최대 개수를 넘을 경우
		{
			InputCount = ItemData.MaxCount;
		}
	}

	else
	{
		int32 SlotIndex = -1;

		for (int32 Index = 0; Index < Spaces.Num(); Index++)
		{
			if (Spaces[Index]->ItemID == ItemData.ItemID && Spaces[Index]->Count == ItemCount)
			{
				SlotIndex = Index;
			}
		}

		if (InputCount > Spaces[SlotIndex]->Count && SlotIndex != -1) // 구입 개수가 최대 개수를 넘을 경우
		{
			InputCount = Spaces[SlotIndex]->Count;
		}
	}

	return InputCount;
}

int32 AInventory::GetItemPrice(FItemTable ItemData, int32 Count, bool IsBuyTep)
{
	if (IsBuyTep) // 구매 탭
	{
		return ItemData.BuyPrice * Count;
	}

	else // 판매 탭
	{
		return ItemData.SellPrice * Count;
	}
}