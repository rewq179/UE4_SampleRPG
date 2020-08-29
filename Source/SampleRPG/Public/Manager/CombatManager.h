// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

UENUM(BlueprintType)
enum class EDamagedType : uint8
{
	EDT_Normal UMETA(DisplayName = "Normal"),
	EDT_KnockBack UMETA(DisplayName = "KnockBack"),
	EDT_Stun UMETA(DisplayName = "Stun"),
	EDT_Poison UMETA(DisplayName = "Poison"),
	EDT_Frostbite UMETA(DisplayName = "Frostbite"),
	EDT_None UMETA(DisplayName = "None"),

	EDT_MAX
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

	void ApplyDamage(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser, EDamagedType DamagedType, bool bIsPlayerDamaged);
	void MonsterDeath(class AMonster* Monster);

	void AddTargetMonster(class AMonster* Monster);
	void AddWidgetMonster(class AMonster* Monster);
	void RemoveWidgetMonster(class AMonster* Monster);
};
