// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "PatternManager.generated.h"

UCLASS()
class SAMPLERPG_API APatternManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatternManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatternManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PatternManager|Properties")
	TMap<int32, TSubclassOf<class APattern>> PatternMap;

public:	
	FPatternTable GetPatternData(int32 PatternID);
	void SetPatternData(class APattern* Pattern, int32 PatternID);

	class APattern* CreatePatternActor(int32 PatternID);
};
