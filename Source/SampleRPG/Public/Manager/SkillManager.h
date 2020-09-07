// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "SkillManager.generated.h"

/**
 * ��ų(����, �����)�� ������ ��Ƴ��� Ŭ������.
*/

UCLASS()
class SAMPLERPG_API ASkillManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillManager|Managerclass")
	class AGameManager* GameManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillManager|Skills")
	TMap<int32, FSkillTable> SkillMap;
	
public:	
	FSkillTable GetSkillData(int32 SkillID);
	void SetSkillData(int32 SkillID);
	void SetSkillDataAll();

	float GetSkillDamage(int32 SkillID);
};
