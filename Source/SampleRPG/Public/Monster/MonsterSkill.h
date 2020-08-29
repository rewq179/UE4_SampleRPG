// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "MonsterSkill.generated.h"

UCLASS()
class SAMPLERPG_API AMonsterSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSkill();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterSkill|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterSkill|Properties")
	class AMonster* Monster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterSkill|SkillTable")
	TMap<int32, class ASkill*> SkillMaps;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void AddSkill(int32 SkillID);
	void ApplySkillDamage(int32 SkillID);

};
