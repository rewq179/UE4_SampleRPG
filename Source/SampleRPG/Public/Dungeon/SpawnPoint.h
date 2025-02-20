// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

/**
* 던전에서 몬스터가 생성되는 위치이다.
*/


UCLASS()
class SAMPLERPG_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnPoint|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint|Point")
	int32 PointID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
