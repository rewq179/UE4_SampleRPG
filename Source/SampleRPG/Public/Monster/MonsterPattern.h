// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/DataTableManager.h"

#include "MonsterPattern.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPatternEndDelegate);

/**
 * 몬스터의 일반과 차징이아닌 패턴 공격만을 분리해놓은 클래스다.
 * 몬스터는 A라는 패턴 공격을 N개 보유할 수 있다.
 */

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

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Components")
	class AMonster* Monster;

	// Pattern //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Pattern")
	TArray<FGatherPattern> Patterns;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Pattern")
	FGatherPattern SelectPattern;

	// Combat //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Combat")
	class AMainPlayer* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Combat")
	bool bCanApplyPatternDamage;
	
public:
	void AddPattern(int32 PatternID); 
	FGatherPattern GetGatherPattern(int32 PatternID, int32 Count); // 한번에 여러발 쏠 수 있기 때문

	// Anim Blueprint //

	UFUNCTION(BlueprintCallable)
	void PatternAnimStart(); // 패턴의 시작 시점

	UFUNCTION(BlueprintCallable)
	void PatternNotifyField(); // 패턴이 적용될 범위를 미리 알려주는 시점

	UFUNCTION(BlueprintCallable)
	void PatternDamageField(); // 패턴의 데미지가 적용되는 시점

	UFUNCTION(BlueprintCallable)
	void PatternApplyBuff();

	UFUNCTION(BlueprintCallable)
	void PatternAnimEnd(); // 패턴의 종료 시점
	FOnPatternEndDelegate OnPatternEnd;

	// CombatManager //

	UFUNCTION(BlueprintCallable)
	void ApplyPatternDamageToTarget(APattern* Pattern); // 패턴 데미지를 CombatManager에 적용하는 함수
};
