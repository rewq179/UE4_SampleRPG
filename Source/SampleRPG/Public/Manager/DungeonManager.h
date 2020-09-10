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

	/* DataTable */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DungeonManager|DataTable")
	TMap<int32, FDungeonTable> DungeonDataMap;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Components")
	class AMainPlayer* MainPlayer;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonManager|Components")
	TMap<int32, class ASpawnPoint*> SpawnPointMap;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	FDungeonTable GetDungeonData(int32 DungeonID);
	void SetDungeonData(int32 DungeonID);
	void SetDungeonDataAll();

	void StringToIntArray(int32 DungeonID, FString Data);

	// Monster Spawn //

	UFUNCTION(BlueprintCallable)
	void AddSpawnPoint(int32 Index, class ASpawnPoint* SpawnPoint);
	
	void CheckTrigger(int32 TriggerID);
	void CheckTriggerAll();
};
