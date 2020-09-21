// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster.h"

#include "Manager/CombatManager.h"

#include "PlayerCombat.generated.h"

/**
 * 플레이어의 공격에 관한 함수와 변수들을 모아놓음.
*/

UCLASS()
class SAMPLERPG_API APlayerCombat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerCombat();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|ManagerClass")
	class ACombatManager* CombatManager;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Components")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Components")
	class APlayerStatus* PlayerStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Components")
	class AInventory* Inventory;

	// Properties //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCombat|Properties")
	class UAnimMontage* CombatMontage;

	// Combo Attack //

	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Combat")
	int32 ComboCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Combat")
	float DamageConst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Combat")
	float ComboLifeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Combat")
	bool bCanComboAttack;

	/* Combat */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Combat")
	EAttackType AttackType;

	FORCEINLINE void SetAttackType(EAttackType AttackType) { this->AttackType = AttackType; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Combat")
	TArray<class AMonster*> TargetMonsters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|Combat")
	class AMonster* TargetMonster;

	FORCEINLINE void AddTargetMonster(AMonster* Monster) { TargetMonsters.AddUnique(Monster); }

	// HUD : Monster Info(Boss > Elite > Normal) //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCombat|HUD")
	TArray<class AMonster*> WidgetMonsters;

	FORCEINLINE void AddWidgetMonster(AMonster* Monster) { WidgetMonsters.AddUnique(Monster); UpdateWidgetMonster(); }
	FORCEINLINE void RemoveWidgetMonster(AMonster* Monster) { WidgetMonsters.Remove(Monster); UpdateWidgetMonster(); }

	// Player Interp //

	float InterpSpeed;
	bool bIsInterp;

	
public:	

	// ComboAttack //

	UFUNCTION(BlueprintCallable)
	void SetComboValue(int32 DamageIndex);

	void CheckComboTimer();

	// Anim Blueprint & Montage //

	void PlayAttackAnim(int32 AttackNumber, float PlayRate = 1.f); // 공격 애니메이션(몽타주) 재생 0:Roll Attack 1~3 : ComboAttack
	
	UFUNCTION(BlueprintCallable)
	void AttackAnimStart(); // CombatCollision 활성화 시점

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTarget(); // 데미지 주는 시점
	
	UFUNCTION(BlueprintCallable)
	void AttackAnimEnd(); // CombatCollision 비활성화 시점

	void PlayMontage(FName AnimName, float PlayRate);

	// HUD : Monster Info(Boss > Elite > Normal) //

	void UpdateWidgetMonster();

	// Player Interp //

	void InterpToMonster(float DeltaTime); // 캐릭터가 공격대상을 바라보게함

	FRotator GetTargetDirection(FVector Target);
};
