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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|ManagerClass")
	class ALevelManager* LevelManager;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Components")
	class AMainPlayer* MainPlayer;

	/* DataTable */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DungeonManager|DataTable")
	TMap<int32, FDungeonTable> DungeonDataMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DungeonManager|DataTable")
	TMap<int32, FTriggerTable> TriggerDataMap;

	/* Revive */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Revive")
	int32 ReviveTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Revive")
	int32 ReviveCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Revive")
	float LifeTime;

	FTimerHandle TimerHandle;

	/* Dungeon */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	int32 CurDungeonID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	TMap<int32, class ASpawnPoint*> SpawnPointMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Dungeon")
	TMap<int32, class ABlockingTrigger*> BlockingTriggerMap;

	/* Triggers */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Trigger")
	TArray<FTriggerTable> CurTriggerMaps;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Trigger")
	int32 SelectID; // Trigger ID

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	/* Dungeon Playing Data */
	void SetReviveData();

	void DecreaseReviveTime();
	
	UFUNCTION(BlueprintCallable)
	void StopTimer();

	/* Add Dungeon's Spawn Point, Block */

	void AddSpawnPoint(int32 Index, class ASpawnPoint* SpawnPoint);
	void AddBlockingTrigger(int32 Index, class ABlockingTrigger* BlockingTrigger);

	/* Dungeon's Trigger */

	void SetDungeonTrigger();

	void CheckTrigger(int32 TargetID);

	void ActiveInstantTrigger();

	bool CanActiveTrigger();

	/* Active Trigger */

	void SetActiveTrigger();

	void GiveClearReward();

	/* Dungeon HUD */
	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveReviveHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDungeonClearHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveDungeonHUD(class ADungeonEnter* DungeonEnter);
};
