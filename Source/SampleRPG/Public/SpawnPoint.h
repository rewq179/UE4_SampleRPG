// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class SAMPLERPG_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint|Location")
	int32 PointID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpawnPoint|Location")
	FVector SpawnLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetLocation();
	
};
