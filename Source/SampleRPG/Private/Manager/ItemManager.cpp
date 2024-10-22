// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/ItemManager.h"
#include "Manager/GameManager.h"
#include "Item/Item.h"

#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AItemManager::AItemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FItemTable AItemManager::GetItemData(int32 ItemID)
{
	AItem* Item = ItemMap[ItemID].GetDefaultObject();

	Item->ItemID = ItemID;
	SetItemData(Item, ItemID);

	return Item->ItemData;
}

void  AItemManager::SetItemData(AItem* Item, int32 ItemID)
{
	FItemTable* ItemTableData = GameManager->DataTableManager->GetItemTableData(ItemID);
	
	if (ItemTableData)
	{
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
		Item->ItemData.DurationTime = (*ItemTableData).DurationTime;
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
	auto Item = GetWorld()->SpawnActor<AItem>(ItemMap[ItemID]);

	Item->Count = Count;
	Item->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	SetItemData(Item, ItemID);

	return Item;
}

void AItemManager::SpawnItemActor(int32 ItemID, int32 Count, FVector Location)
{
	AItem* Item = CreateItemActor(ItemID, Count);

	Item->SetActorLocation(Location);
	Item->SetActiveText(true);
}

FRewardTable AItemManager::GetRewardData(int32 RewardID)
{
	return RewardDataMap[RewardID];
}

void AItemManager::SetRewardData(int32 RewardID)
{
	FRewardRawTable* RewardRawTableData = GameManager->DataTableManager->GetRewardRawTableData(RewardID);

	if (RewardRawTableData)
	{
		RewardDataMap[RewardID].RewardID = (*RewardRawTableData).RewardID;
		RewardDataMap[RewardID].Name = (*RewardRawTableData).Name;

		RewardDataMap[RewardID].bIsPackaging= (*RewardRawTableData).bIsPackaging;
		RewardDataMap[RewardID].Percent = (*RewardRawTableData).Percent;

		RewardDataMap[RewardID].Boxes.Add(RewardBox((*RewardRawTableData).ID_0, (*RewardRawTableData).Count_0, (*RewardRawTableData).Percent_0));

		if ((*RewardRawTableData).ID_1 != -1)
		{
			RewardDataMap[RewardID].Boxes.Add(RewardBox((*RewardRawTableData).ID_0, (*RewardRawTableData).Count_1, (*RewardRawTableData).Percent_1));
		}

		if ((*RewardRawTableData).ID_2 != -1)
		{
			RewardDataMap[RewardID].Boxes.Add(RewardBox((*RewardRawTableData).ID_2, (*RewardRawTableData).Count_2, (*RewardRawTableData).Percent_2));
		}

		if ((*RewardRawTableData).ID_3 != -1)
		{
			RewardDataMap[RewardID].Boxes.Add(RewardBox((*RewardRawTableData).ID_3, (*RewardRawTableData).Count_3, (*RewardRawTableData).Percent_3));
		}

		if ((*RewardRawTableData).ID_4 != -1)
		{
			RewardDataMap[RewardID].Boxes.Add(RewardBox((*RewardRawTableData).ID_4, (*RewardRawTableData).Count_4, (*RewardRawTableData).Percent_4));
		}
		
		RewardDataMap[RewardID].Pack_ID = (*RewardRawTableData).Pack_ID;
		RewardDataMap[RewardID].Pack_Percent = (*RewardRawTableData).Pack_Percent;

	}
}

void AItemManager::SetRewardDataAll()
{
	for (int32 RewardID = 0; RewardID < RewardDataMap.Num(); RewardID++)
	{
		SetRewardData(RewardID);
	}
}


FRewardBox AItemManager::RewardBox(int32 ID, int32 Count, float Percent)
{
	FRewardBox RewardBox;
	
	RewardBox.BoxID = ID;
	RewardBox.BoxCount = Count;
	RewardBox.BoxPercent = Percent;

	return RewardBox;
}

void AItemManager::GetMonsterItem(int32 ProductID, int32 RewardID, FVector Location)
{
	if (RewardID != -1)
	{
		GetItemInRewardBox(ProductID, Location);
	}

	Location += FVector(20.f, 20.f, 0.f);

	if (RewardID != -1)
	{
		GetItemInRewardBox(RewardID, Location);
	}
}

void AItemManager::GetItemInRewardBox(int32 RewardID, FVector Location)
{
	FRewardTable RewardData = GetRewardData(RewardID);

	int32 BoxIndex = GetBoxIndex(RewardData);

	if (BoxIndex >= 0) // 당첨
	{
		SpawnItemActor(RewardData.Boxes[BoxIndex].BoxID, RewardData.Boxes[BoxIndex].BoxCount, GetGroundSurfaceLocation(Location));
	}

	else if (BoxIndex == -2) // 패키지 돌리기
	{
		RewardData = GetRewardData(RewardData.Pack_ID);

		BoxIndex = GetBoxIndex(RewardData);

		SpawnItemActor(RewardData.Boxes[BoxIndex].BoxID, RewardData.Boxes[BoxIndex].BoxCount, GetGroundSurfaceLocation(Location));
	}
}

int32 AItemManager::GetBoxIndex(FRewardTable RewardData)
{
	int32 Rand = FMath::RandRange(0, (int)RewardData.Percent * 100);
	int32 High = 0;

	for (int32 BoxIndex = 0; BoxIndex < RewardData.Boxes.Num(); BoxIndex++)
	{
		High += RewardData.Boxes[BoxIndex].BoxPercent * 100;

		if (Rand < High) // 숫자 In
		{
			UE_LOG(LogTemp, Log, TEXT("Num : %d <= %d"), Rand, High);

			if (RewardData.Boxes[BoxIndex].BoxID == -2) // 꽝
			{
				UE_LOG(LogTemp, Log, TEXT("FAIL!!"));

				return -1; // 꽝
			}

			return BoxIndex;
		}
	}

	return -2; // 패키지 다시 돌리기
}

FVector AItemManager::GetGroundSurfaceLocation(FVector Loctaion)
{
	FHitResult Hit;

	FVector Start = Loctaion;
	FVector End = Start - FVector(0.f, 0.f, 500.f);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		if (Hit.bBlockingHit)
		{
			return Hit.ImpactPoint;
		}
	}

	return FVector(0.f);
}