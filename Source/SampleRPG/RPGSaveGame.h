// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MainPlayer.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	
public:
	URPGSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FPlayerStatusTable PlayerStatus;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator Rotation;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|SaveData")
	TMap<int32, int32> InventoryItemMap;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|SaveData")
	TArray<int32> EquipmentItem;
};
