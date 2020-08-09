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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGameManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FPlayerStatTable PlayerStat;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SaveGameManager|Level")
	FString LevelName;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameManager|SaveData")
	TMap<int32, int32> InventoryItemMap;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameManager|SaveData")
	TArray<int32> EquipmentItem;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGameManager|SaveData")
	TMap<int32, bool> PlayerQuestMap;
};
