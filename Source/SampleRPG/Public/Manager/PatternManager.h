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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatternManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PatternManager|PatternData")
	TMap<int32, TSubclassOf<class APattern>> PatternMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FPatternTable GetPatternData(int32 PatternID);

	void SetPatternData(class APattern* Pattern, int32 PatternID);
	FPatternShape PatternShape(EShape Shape, float X, float Y);

	class APattern* CreatePatternActor(int32 PatternID);
};
