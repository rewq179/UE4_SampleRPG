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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NpcManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NpcManager|NpcData")
	TMap<int32, FNpcTable> NpcDataMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NpcManager|Npc")
	TMap<int32, class ANpcController*> NpcMap;

	void SetNpcDataAll();
	void SetNpcData(int32 NpcID);

	FNpcTable GetNpcData(int32 NpcID);

	void CheckNpcSymbolAll();
	void CheckNpcSymbol(int32 NpcID);
	void SetNpcSymbol(ESymbolType SymbolType, int32 NpcID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	
	
};
