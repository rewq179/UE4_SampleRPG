// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "DungeonManager.generated.h"

/**
* 던전과 트리거의 DataTable을 보관하며, 현재 던전의 트리거(Block,Spawn,Clear)들을 관리한다.
*/

UCLASS()
class SAMPLERPG_API ADungeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonManager();
	
	/* Manager Class */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|ManagerClass")
	class AMonsterManager* MonsterManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|ManagerClass")
	class ALevelManager* LevelManager;

	// Properties //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Properties")
	class AMainPlayer* MainPlayer;

	/* DataTable */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DungeonManager|DataTable")
	TMap<int32, FDungeonTable> DungeonDataMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DungeonManager|DataTable")
	TMap<int32, FTriggerTable> TriggerDataMap;

	/* Current Dungeon Data */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	int32 CurDungeonID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	int32 CurTriggerID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	TArray<FTriggerTable> CurTriggerMaps;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	TMap<int32, class ASpawnPoint*> SpawnPointMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	TMap<int32, class ABlockingTrigger*> BlockingTriggerMap;

	/* Revive */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Revive")
	int32 ReviveTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Revive")
	int32 ReviveCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Revive")
	float LifeTime;

	FTimerHandle TimerHandle;

public:	
	UFUNCTION(BlueprintCallable)
	void LoadLevel(FString LevelName);
	
	/* Getter & Setter */

	FDungeonTable GetDungeonData(int32 DungeonID);
	void SetDungeonData(int32 DungeonID);
	void SetDungeonDataAll();
	void StringToIntDungeonArray(int32 DungeonID, FString Data, int32 Column);

	FTriggerTable GetTriggerData(int32 TriggerID);
	void SetTriggerData(int32 TriggerID);
	void SetTriggerDataAll();
	void StringToIntTriggerArray(int32 TriggerID, FString Data, int32 Column);

	/* Add Dungeon's Spawn Point, Block */

	void AddSpawnPoint(int32 Index, class ASpawnPoint* SpawnPoint);
	void AddBlockingTrigger(int32 Index, class ABlockingTrigger* BlockingTrigger);

	/* Dungeon's Trigger */

	void SetDungeonTrigger();

	void CheckCurTrigger(int32 TargetID);

	bool CanActiveTrigger();

	/* Active Trigger */
	
	void ActiveInstantTrigger();

	void ActiveTrigger();

	void GiveClearReward();

	/* Revive */

	void StartReviveTimer();

	void CheckReviveTime();
	
	UFUNCTION(BlueprintCallable)
	void StopReviveTimer();

	/* Dungeon HUD */
	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveReviveHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDungeonClearHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDungeonHUD(class ADungeonEnter* DungeonEnter);
};
