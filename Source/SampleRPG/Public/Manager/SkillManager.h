// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "SkillManager.generated.h"

UCLASS()
class SAMPLERPG_API ASkillManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillManager|SkillData")
	TMap<int32, TSubclassOf<class ASkill>> SkillMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	class ASkill* CreateSkillActor(int32 SkillID);
	void SetSkillData(class ASkill* Skill, int32 SkillID);
	FSkillShape SkillShape(EShape Shape, float X, float Y);

	
};
