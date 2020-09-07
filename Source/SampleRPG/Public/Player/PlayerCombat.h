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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|ManagerClass")
	class ACombatManager* CombatManager;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Components")
	class AMainPlayer* MainPlayer;

	// Properties //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class UAnimMontage* CombatMontage;

	// Combat //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	EAttackType AttackType;

	FORCEINLINE void SetAttackType(EAttackType AttackType) { this->AttackType = AttackType; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	TArray<class AMonster*> TargetMonsters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	class AMonster* TargetMonster;

	FORCEINLINE void AddTargetMonster(AMonster* Monster) { TargetMonsters.AddUnique(Monster); }

	// HUD : Monster Info(Boss > Elite > Normal) //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|HUD")
	TArray<class AMonster*> WidgetMonsters;

	FORCEINLINE void AddWidgetMonster(AMonster* Monster) { WidgetMonsters.AddUnique(Monster); UpdateWidgetMonster(); }
	FORCEINLINE void RemoveWidgetMonster(AMonster* Monster) { WidgetMonsters.Remove(Monster); UpdateWidgetMonster(); }

	// Player Interp //

	float InterpSpeed;
	bool bIsInterp;

	
public:	



	// Anim Blueprint & Montage //

	void PlayAttackAnim(); // 공격 애니메이션(몽타주) 재생 

	UFUNCTION(BlueprintCallable)
	void AttackAnimStart(); // CombatCollision 활성화 시점

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTarget(); // 데미지 주는 시점
	
	UFUNCTION(BlueprintCallable)
	void AttackAnimEnd(); // CombatCollision 비활성화 시점

	void PlayMontage(FName AnimName, float PlayRate);

	// HUD : Monster Info(Boss > Elite > Normal) //

	void UpdateWidgetMonster();

	int32 GetPriorityByClass(EMonsterClass Class);
	
	// Player Interp //

	void InterpToMonster(float DeltaTime); // 캐릭터가 공격대상을 바라보게함

	FRotator GetTargetDirection(FVector Target);
};
