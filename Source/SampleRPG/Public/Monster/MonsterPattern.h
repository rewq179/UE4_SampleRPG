// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/DataTableManager.h"

#include "MonsterPattern.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPatternEndDelegate);

/**
 * ������ �Ϲݰ� ��¡�̾ƴ� ���� ���ݸ��� �и��س��� Ŭ������.
 * ���ʹ� A��� ���� ������ N�� ������ �� �ִ�.
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
	FGatherPattern GetGatherPattern(int32 PatternID, int32 Count); // �ѹ��� ������ �� �� �ֱ� ����

	// Anim Blueprint //

	UFUNCTION(BlueprintCallable)
	void PatternAnimStart(); // ������ ���� ����

	UFUNCTION(BlueprintCallable)
	void PatternNotifyField(); // ������ ����� ������ �̸� �˷��ִ� ����

	UFUNCTION(BlueprintCallable)
	void PatternDamageField(); // ������ �������� ����Ǵ� ����

	UFUNCTION(BlueprintCallable)
	void PatternApplyBuff();

	UFUNCTION(BlueprintCallable)
	void PatternAnimEnd(); // ������ ���� ����
	FOnPatternEndDelegate OnPatternEnd;

	// CombatManager //

	UFUNCTION(BlueprintCallable)
	void ApplyPatternDamageToTarget(APattern* Pattern); // ���� �������� CombatManager�� �����ϴ� �Լ�
};
