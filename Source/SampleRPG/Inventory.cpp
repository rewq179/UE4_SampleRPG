// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "ItemStorage.h"
#include "RPGSaveGame.h"

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

	if (ItemStorageBP)
	{
		ItemStorage = GetWorld()->SpawnActor<AItemStorage>(ItemStorageBP);
	}
}

void AInventory::AddItem(AItem* Item)
{
	/*
		���� :  �κ��丮�� �Ⱦ �������� �����ϴ��� Ȯ���Ѵ�.

		if) O : Item�� Count�� �����Ѵ�. Count�� Max�� �Ѿ�� ���ο� ������ Item�� ��ġ�Ѵ�.
		if) X : ���ο� ������ �ش� Item�� ��ġ�Ѵ�.
	*/
	int32 InvIndex = GetInventoryIndex(Item);
	int32 TotalCount = Item->Count;
	int32 MaxCount = Item->ItemTableValue.MaxCount;

	Item->SetItemState(EItemState::EIS_Inv);
	Item->ItemOwner = MainPlayer;
	Item->SetActorLocation(FVector(0.f));

	if (InvIndex != -1) // �κ��丮�� �������� �����Ѵ�
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

	else // �κ��丮�� �������� ����
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
			Spaces.Add(Item);
		}
	}


	if (Item->ItemTableValue.ItemClass == EItemClass::EIC_Equip)
	{
		Item->IgnoreStaticMesh();
		Item->SetCombatCollisionEnabled(false);
	}
	
	UpdateInventorySlot();
}

int32 AInventory::GetInventoryIndex(AItem* Item)
{
	for (int32 InvIndex = 0; InvIndex != Spaces.Num(); InvIndex++) // �ش� �������� �ֳ�?
	{
		if (Spaces[InvIndex]->ItemID == Item->ItemID)
		{
			if (Spaces[InvIndex]->Count < Item->ItemTableValue.MaxCount)
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
		AItem* DevideItem = CreateItemActor(Item->ItemID);

		if (TotalCount > Item->ItemTableValue.MaxCount)
		{
			DevideItem->Count = Item->ItemTableValue.MaxCount;
		}

		else
		{
			DevideItem->Count = TotalCount;
		}

		Spaces.Add(DevideItem);
		TotalCount -= Item->ItemTableValue.MaxCount;
	}
}

void AInventory::RemoveItem(AItem* Item)
{
	int32 SlotIndex = Spaces.IndexOfByKey(Item);

	int32 TotalCount = Spaces[SlotIndex]->Count -= Item->Count;

	if (TotalCount > 0)
	{
		Spaces[SlotIndex]->Count -= Item->Count;
	}

	else
	{
		Spaces.RemoveAt(SlotIndex);
	}
}

void AInventory::UseItem(AItem* Item, int32 SlotIndex)
{
	if (Item->Count > 0)
	{
		if (Item->ItemTableValue.ItemClass == EItemClass::EIC_Equip)
		{
			EquipItem(Item, SlotIndex);
		}

		else if (Item->ItemTableValue.ItemClass == EItemClass::EIC_Consume)
		{
			ConsumeItem(Item, SlotIndex);
		}
	}
}

void AInventory::ConsumeItem(AItem* Item, int32 SlotIndex)
{
	Spaces[SlotIndex]->Count--;

	MainPlayer->PlayerStatus->AdjustHP(Item->ItemTableValue.Damage, false);

	if (Item->UseSound)
	{
		UGameplayStatics::PlaySound2D(this, Item->UseSound);
	}
}

void AInventory::EquipItem(AItem* NewItem, int32 InvIndex)
{
	/*
		0:����, 1:����, 2:���, 3:����, 4:�����, 5:�尩, 6:����, 7:�Ź�, 8:�Ǽ� 1, 9:�Ǽ� 2
	*/
	Spaces.RemoveAt(InvIndex);

	int32 EquipIndex = GetEquipmentIndex(NewItem);

	// ������� : #include "Engine/SkeletalMeshSocket.h"

	if (EquipIndex == 0) // ����
	{
		
		const USkeletalMeshSocket* RightHandSocket = MainPlayer->GetMesh()->GetSocketByName("RightHandSocket");

		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(NewItem, MainPlayer->GetMesh());
		}

		MainPlayer->bIsEquip = true;
	}

	else if (EquipIndex == 1) // ����
	{
		const USkeletalMeshSocket* LeftHandSocket = MainPlayer->GetMesh()->GetSocketByName("LeftHandSocket");

		if (LeftHandSocket)
		{
			LeftHandSocket->AttachActor(NewItem, MainPlayer->GetMesh());
		}
	}

	else if (EquipIndex == 8) // ����
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
	switch (Item->ItemTableValue.ItemType)
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
		MainPlayer->PlayerStatus->IncDamage += Item->ItemTableValue.Damage;
		MainPlayer->PlayerStatus->IncDeffence += Item->ItemTableValue.Deffence;
		MainPlayer->PlayerStatus->IncStrength += Item->ItemTableValue.Strength;
		MainPlayer->PlayerStatus->IncDexterity += Item->ItemTableValue.Dexterity;
		MainPlayer->PlayerStatus->IncIntelligence += Item->ItemTableValue.Intelligence;
	}
	
	else
	{
		MainPlayer->PlayerStatus->IncDamage -= Item->ItemTableValue.Damage;
		MainPlayer->PlayerStatus->IncDeffence -= Item->ItemTableValue.Deffence;
		MainPlayer->PlayerStatus->IncStrength -= Item->ItemTableValue.Strength;
		MainPlayer->PlayerStatus->IncDexterity -= Item->ItemTableValue.Dexterity;
		MainPlayer->PlayerStatus->IncIntelligence -= Item->ItemTableValue.Intelligence;
	}
}

void AInventory::SaveInventoryData(URPGSaveGame* SaveGameInstance)
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


void AInventory::LoadInventoryData(URPGSaveGame* LoadGameInstance)
{
	if (ItemStorageBP && ItemStorage)
	{
		Spaces.Empty();

		for (auto InvItem : LoadGameInstance->InventoryItemMap)
		{
			AItem* Item = CreateItemActor(InvItem.Key);
			Item->Count = InvItem.Value;

			AddItem(Item);
		}

		for (int32 SlotIndex = 0; SlotIndex < 10; SlotIndex++)
		{
			Equipments[SlotIndex] = nullptr;

			if (LoadGameInstance->EquipmentItem[SlotIndex] >= 0)
			{
				AItem* Item = CreateItemActor(LoadGameInstance->EquipmentItem[SlotIndex]);
				Item->Count = 1;

				AddItem(Item);
				EquipItem(Item, Spaces.Num() - 1);
			}
		}
	}
}


AItem* AInventory::CreateItemActor(int32 ItemID)
{
	AItem* Item = GetWorld()->SpawnActor<AItem>(ItemStorage->ItemMap[ItemID]);

	return Item;
}