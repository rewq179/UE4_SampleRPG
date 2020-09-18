// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "DungeonManager.generated.h"

/**
* 던전의 TriggerVolume, 몬스터 Spawn, 보상을 관리하는 클래스.
*/

UCLASS()
class SAMPLERPG_API ADungeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|ManagerClass")
	class AMonsterManager* MonsterManager;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Components")
	class AMainPlayer* MainPlayer;

	/* DataTable */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DungeonManager|DataTable")
	TMap<int32, FDungeonTable> DungeonDataMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DungeonManager|DataTable")
	TMap<int32, FTriggerTable> TriggerDataMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	int32 CurDungeonID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	TMap<int32, class ASpawnPoint*> SpawnPointMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	TMap<int32, class ABlockingTrigger*> BlockingTriggerMap;

	/* Triggers */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Trigger")
	TMap<int32, FTriggerTable> CurTriggerMaps;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Trigger")
	int32 SelectID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	FDungeonTable GetDungeonData(int32 DungeonID);
	void SetDungeonData(int32 DungeonID);
	void SetDungeonDataAll();
	void StringToIntDungeonArray(int32 DungeonID, FString Data, int32 Column);

	FTriggerTable GetTriggerData(int32 TriggerID);

	void SetTriggerData(int32 TriggerID);
	void SetTriggerDataAll();
	void StringToIntTriggerArray(int32 TriggerID, FString Data, int32 Column);


	// Monster Spawn //

	void AddSpawnPoint(int32 Index, class ASpawnPoint* SpawnPoint);
	void AddBlockingTrigger(int32 Index, class ABlockingTrigger* BlockingTrigger);

	void SetDungeonTrigger();

	void CheckTrigger(int32 TargetID);
	void CheckTriggerCount(int32 TargetID);

	bool CanActiveTrigger();

	void SetActiveTrigger();
	void SpawnMonsterInPoint(int32 MonsterID, int32 PointID);
	void SetOverlapBlock(int32 BlockID);

	/* Dungeon HUD */

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveClearHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDungeonHUD(FDungeonTable DungeonData);
};
