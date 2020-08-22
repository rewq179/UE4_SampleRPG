// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/DataTableManager.h"

#include "Manager/CombatManager.h"

#include "Monster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChargingEndDelegate);

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Normal UMETA(DisplayName = "Normal"),
	EAT_Charging UMETA(DisplayName = "Charging"),

	EAT_MAX
};

UCLASS()
class SAMPLERPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UParticleSystem* DamagedParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class USoundCue* DamagedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	FTimerHandle TimeHandle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|MonsterTable")
	int32 MonsterID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|MonsterTable")
	FMonsterTable Status;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class ACombatManager* CombatManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Combat")
	class UBoxComponent* CombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	EDamagedType DamagedType;
	FORCEINLINE void SetDamagedType(EDamagedType Type) { DamagedType = Type; }

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	EMonsterState MonsterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AMonsterAI* MonsterAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AMainPlayer* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bCanApplyDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bCanChargingAttack;

	FORCEINLINE void SetChargingAttack() { bCanChargingAttack = true; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetMonsterData();
	void SetMonsterState(EMonsterState State);

	void AttackTarget(class AMainPlayer* Target, EAttackType AttackType);
	int32 GetAttackNumber(EAttackType AttackType);

	void TakeDamage(float DamageAmount, AActor* DamageCauser, EDamagedType DamageType);
	void Death();
	
	void TakeGroggy(float DamageAMount, AActor* DamageCauser);
	void Stun();

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTarget(); // 데미지 주는 시점

	UFUNCTION(BlueprintCallable)
	void AttackAnimEnd();
	FOnAttackEndDelegate OnAttackEnd;

	UFUNCTION(BlueprintCallable)
	void ChargingAnimEnd();
	FOnChargingEndDelegate OnChargingEnd;

	UFUNCTION(BlueprintCallable)
	void DeathAnimEnd();

	void PlayMontage(FName Name, float PlayRate);

	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void SetCombatCollisionEnabled(bool IsActive);
};
