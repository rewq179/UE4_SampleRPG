// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_None UMETA(DisplayName = "None"),
	EAT_KnockDown UMETA(DisplayName = "KnockDown"),
	EAT_Groggy UMETA(DisplayName = "Groggy"),
	EAT_Poison UMETA(DisplayName = "Poison"),
	EAT_Frostbite UMETA(DisplayName = "Frostbite")
};

UENUM(BlueprintType)
enum class EStatusType : uint8
{
	EST_None UMETA(DisplayName = "None"),
	EST_CurHp UMETA(DisplayName = "Cur Health"),
	EST_PerHP UMETA(DisplayName = "Per Health"),
	EST_CurST UMETA(DisplayName = "Cur Stamina"),
	EST_PerST UMETA(DisplayName = "Per Stamina")
};

UCLASS()
class SAMPLERPG_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|ManagerClass")
	class ASkillManager* SkillManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Combat")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Combat")
	class AMonster* CombatMonster;

	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Combat")
	float LifeTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// 데미지 적용 //

	void ApplyDamageHP(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, EAttackType AttackType, bool bIsPlayerDamaged, bool bIsPercent);
	void AdjustDamageHP(AActor* DamagedActor, float BaseDamage, bool bIsPercent);
	
	void ApplyDamageST(AActor* DamagedActor, float BaseDamage, bool bIsPercent);
	float GetPercentBaseDamage(float Percent, float MaxValue);
	
	void MonsterDeath(class AMonster* Monster);

	// 상태이상 적용 //

	void SetDebuffToPlayer(EAttackType AttackType);
	bool bIsLifeTimeOver(float DurationTime);

	void SetPoison();
	void SetFrostbite();

	// HUD //

	void AddTargetMonster(class AMonster* Monster);
	void AddWidgetMonster(class AMonster* Monster);
	void RemoveWidgetMonster(class AMonster* Monster);


};
