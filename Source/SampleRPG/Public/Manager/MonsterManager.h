// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "MonsterManager.generated.h"

UCLASS()
class SAMPLERPG_API AMonsterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterManager|DataTable")
	TMap<int32, TSubclassOf<class AMonster>> MonsterMaps;

public:	
	FMonsterTable GetMonsterData(int32 MonsterID);
	void SetMonsterData(class AMonster* Monster, int32 MonsterID);

	void SpawnMonsterInLocation(int32 MonsterID, FVector Location);

	class AMonster* CreateMonsterActor(int32 MonsterID);
	
};


