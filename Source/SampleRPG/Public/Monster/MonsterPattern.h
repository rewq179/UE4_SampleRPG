// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/DataTableManager.h"

#include "MonsterPattern.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPatternEndDelegate);

USTRUCT(BlueprintType)
struct FGatherPattern
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|PatternTable")
	TArray<class APattern*> Patterns;
};

UCLASS()
class SAMPLERPG_API AMonsterPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterPattern();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|ManagerClass")
	class ACombatManager* CombatManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Components")
	class AMonster* Monster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Pattern")
	TArray<FGatherPattern> Patterns;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Pattern")
	FGatherPattern SelectPattern;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Combat")
	class AMainPlayer* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Combat")
	bool bCanApplyPatternDamage;
	
public:
	void AddPattern(int32 PatternID); 
	FGatherPattern GetGatherPattern(int32 PatternID, int32 Count); // 한번에 여러발 쏠 수 있기 때문

	UFUNCTION(BlueprintCallable)
	void PatternAnimStart();

	UFUNCTION(BlueprintCallable)
	void PatternNotifyField();

	UFUNCTION(BlueprintCallable)
	void PatternDamageField();

	UFUNCTION(BlueprintCallable)
	void PatternApplyBuff();

	UFUNCTION(BlueprintCallable)
	void PatternAnimEnd();
	FOnPatternEndDelegate OnPatternEnd;

	UFUNCTION(BlueprintCallable)
	void ApplyPatternDamageToTarget(APattern* Pattern);

};
