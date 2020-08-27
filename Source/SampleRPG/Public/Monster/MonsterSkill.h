// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSkill.generated.h"

UCLASS()
class SAMPLERPG_API AMonsterSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSkill();

	float SkillCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterSkill|Properties")
	TArray<class UParticleSystem*> SkillParticles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
