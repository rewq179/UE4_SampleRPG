// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Manager/DungeonManager.h"

#include "RGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInstance|Components")
	FDungeonTable CurDungeon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInstance|Components")
	FString SlotName;
};
