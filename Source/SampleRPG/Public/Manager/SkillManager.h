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

	class UDataTable* SkillTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillManager|SkillTable")
	TMap<int32, FSkillTable> SkillMap;


protected:


public:	
	void SetAllSkillData();
	void SetSkillData(int32 SkillID);

	FSkillTable GetSkillData(int32 SkillID);

	
	
};
