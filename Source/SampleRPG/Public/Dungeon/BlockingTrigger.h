// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockingTrigger.generated.h"

UCLASS()
class SAMPLERPG_API ABlockingTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockingTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlockingTrigger|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlockingTrigger|Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlockingTrigger|Block")
	int32 BlockID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetOverlap();
};
