// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/DataTableManager.h"
#include "Npc/NpcController.h"
#include "NpcManager.generated.h"

/**
 * NPC에 대한 정보, 월드상 존재하는 NPC, 플레이어의 퀘스트 진행도에 따른 Symbol 변화를 체크하기 위한 클래스다.
*/

UCLASS()
class SAMPLERPG_API ANpcManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANpcManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NpcManager|ManagerClass")
	class AGameManager* GameManager;

	// DataTable //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NpcManager|DataTable")
	TMap<int32, FNpcTable> NpcDataMap;

	// //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NpcManager|Npc")
	TMap<int32, class ANpcController*> NpcMap;
	
public:	
	FNpcTable GetNpcData(int32 NpcID);
	void SetNpcData(int32 NpcID);
	void SetNpcDataAll();

	void CheckQuestSymbol(int32 NpcID);
	void SetQuestSymbol(int32 NpcID, ESymbolType SymbolType);

	void CheckQuestSymbolAllNPC();

};
