// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/DataTableManager.h"

#include "MonsterPattern.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPatternEndDelegate);

UCLASS()
class SAMPLERPG_API AMonsterPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterPattern();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterPattern|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterPattern|Properties")
	class ACombatManager* CombatManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Properties")
	class AMonster* Monster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|PatternTable")
	TMap<int32, class APattern*> PatternMaps;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Combat")
	class APattern* SelectedPattern;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Combat")
	class AMainPlayer* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MonsterPattern|Combat")
	bool bCanApplyPatternDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void AddPattern(int32 PatternID);

	UFUNCTION(BlueprintCallable)
	void PatternAnimStart();

	UFUNCTION(BlueprintCallable)
	void PatternNotifyField();

	UFUNCTION(BlueprintCallable)
	void PatternDamageField();

	UFUNCTION(BlueprintCallable)
	void PatternAnimEnd();
	FOnPatternEndDelegate OnPatternEnd;

	UFUNCTION(BlueprintCallable)
	void ApplyPatternDamageToTarget();

};
