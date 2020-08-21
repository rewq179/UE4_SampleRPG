// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster.h"

#include "Manager/CombatManager.h"

#include "PlayerCombat.generated.h"

UCLASS()
class SAMPLERPG_API APlayerCombat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerCombat();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class ACombatManager* CombatManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	TArray<class AMonster*> TargetMonsters;

	FORCEINLINE void AddTargetMonster(AMonster* Monster) { TargetMonsters.AddUnique(Monster); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	TArray<class AMonster*> WidgetMonsters;

	FORCEINLINE void AddWidgetMonster(AMonster* Monster) { WidgetMonsters.AddUnique(Monster); UpdateWidgetMonster(); }
	FORCEINLINE void RemoveWidgetMonster(AMonster* Monster) { WidgetMonsters.Remove(Monster); UpdateWidgetMonster(); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	class AMonster* TargetMonster;

	void UpdateWidgetMonster();
	int32 GetPriorityByClass(EMonsterClass Class);

	void PlayAttackAnim(); // 공격 애니메이션(몽타주) 재생 관리

	// 아래는 애니메이션 Notify와 연동된 함수들

	float InterpSpeed;
	bool bIsInterp;

	FRotator GetTargetDirection(FVector Target);
	void InterpToMonster(float DeltaTime); // 캐릭터가 공격대상을 바라보게함

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	EDamagedType DamagedType;

	FORCEINLINE void SetDamagedType(EDamagedType DamagedType) { this->DamagedType = DamagedType; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTarget(); // 데미지 주는 시점
	
	UFUNCTION(BlueprintCallable)
	void AttackAnimStart(); // CombatCollision 활성화
	UFUNCTION(BlueprintCallable)
	void AttackAnimEnd(); // CombatCollision 비활성화

	void PlayMontage(FName Name, float PlayRate);
	
};
