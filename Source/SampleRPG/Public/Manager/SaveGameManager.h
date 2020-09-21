// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Player/PlayerStatus.h"
#include "SaveGameManager.generated.h"

UCLASS()
class SAMPLERPG_API USaveGameManager : public USaveGame
{
	GENERATED_BODY()
	
public:
	USaveGameManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGameManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameManager|GameData")
	FString SlotName;
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameManager|GameData")
	uint32 SlotIndex;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameManager|GameData")
	FPlayerStatTable PlayerStat;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameManager|GameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameManager|GameData")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SaveGameManager|GameData")
	FString LevelName;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameManager|GameData")
	TMap<int32, int32> InventoryItemMap;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameManager|GameData")
	TArray<int32> EquipmentItem;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameManager|GameData")
	TMap<int32, bool> PlayerQuestMap;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameManager|GameData")
	TMap<int32, int32> PlayerQuestCountMap;
};
