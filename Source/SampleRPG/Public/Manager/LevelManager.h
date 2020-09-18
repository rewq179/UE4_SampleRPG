// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class SAMPLERPG_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelManager|ManagerClass")
	class AGameManager* GameManager;

	// Transition //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelManager|Transition")
	class UBoxComponent* Transition;

	class UBillboardComponent* Billboard;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void LoadLevel(FString LevelName);
};
