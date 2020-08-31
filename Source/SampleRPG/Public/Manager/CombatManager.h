// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_None UMETA(DisplayName = "None"),
	EAT_Normal UMETA(DisplayName = "Normal"),
	EAT_KnockBack UMETA(DisplayName = "KnockBack"),
	EAT_Stun UMETA(DisplayName = "Stun"),
	EAT_Poison UMETA(DisplayName = "Poison"),
	EAT_Frostbite UMETA(DisplayName = "Frostbite"),

	EAT_MAX
};

UENUM(BlueprintType)
enum class EDamagedType : uint8
{
	EDT_None UMETA(DisplayName = "None"),
	EDT_CurHp UMETA(DisplayName = "Cur Health"),
	EDT_PerHP UMETA(DisplayName = "Per Health"),
	EDT_CurST UMETA(DisplayName = "Cur Stamina"),
	EDT_PerST UMETA(DisplayName = "Per Stamina"),
	
	EDT_Max
};


UCLASS()
class SAMPLERPG_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Properties")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatManager|Combat")
	class AMonster* CombatMonster;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void ApplyDamageHP(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, EAttackType AttackType, bool bIsPlayerDamaged, bool bIsPercent);
	void ApplyDamageST(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, EAttackType AttackType, bool bIsPercent);
	void MonsterDeath(class AMonster* Monster);
	float GetPercentBaseDamage(float Percent, float MaxHP);

	void AddTargetMonster(class AMonster* Monster);
	void AddWidgetMonster(class AMonster* Monster);
	void RemoveWidgetMonster(class AMonster* Monster);
};
