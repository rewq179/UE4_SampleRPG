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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level|Transition")
	FString LevelName;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|SaveData")
	TMap<int32, int32> InventoryItemMap;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|SaveData")
	TArray<int32> EquipmentItem;
};
