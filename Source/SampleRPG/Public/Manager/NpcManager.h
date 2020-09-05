// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/DataTableManager.h"
#include "Npc/NpcController.h"
#include "NpcManager.generated.h"

UCLASS()
class SAMPLERPG_API ANpcManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANpcManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NpcManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NpcManager|Npc")
	TMap<int32, FNpcTable> NpcDataMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NpcManager|Npc")
	TMap<int32, class ANpcController*> NpcMap;
	
public:	
	FNpcTable GetNpcData(int32 NpcID);
	void SetNpcData(int32 NpcID);
	void SetNpcDataAll();

	void SetNpcSymbol(ESymbolType SymbolType, int32 NpcID);

	void CheckNpcSymbol(int32 NpcID);
	void CheckNpcSymbolAll();

};
