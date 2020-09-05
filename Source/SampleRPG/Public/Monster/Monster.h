// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/DataTableManager.h"

#include "Manager/CombatManager.h"

#include "Monster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChargingEndDelegate);

UCLASS()
class SAMPLERPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|ManagerClass")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|ManagerClass")
	class ACombatManager* CombatManager;

	// Components //

	UPROPERTY(EditDefaultsOnly, Category = "Monster|Components")
	TSubclassOf<class AMonsterPattern> MonsterPatternBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Components")
	class AMonsterPattern* MonsterPattern;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Components")
	class AMonsterAI* MonsterAI;

	// Properties //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UParticleSystem* DamagedParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class USoundCue* DamagedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UAnimMontage* CombatMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UBoxComponent* CombatRightCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UBoxComponent* CombatLeftCollision;

	FTimerHandle TimeHandle;

	// DataTable //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|DataTable")
	int32 MonsterID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|DataTable")
	FMonsterTable Status;

	// Combat //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AMainPlayer* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster|Combat")
	EHandType HandType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	EAttackType AttackType;
	FORCEINLINE void SetAttackType(EAttackType Type) { AttackType = Type; }
	

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
	void ParseStringToInt(FString Data);

	void AttackTarget(class AMainPlayer* Target, EAttackClass AttackClass, int32 AttackNumber);

	// Monster Status //

	void TakeDamageHP(float DamageAmount, AActor* DamageCauser, EAttackType DamageType);
	void Death();
	
	void TakeGroggy(float DamageAMount, AActor* DamageCauser);
	void Stun();

	// Animation Blue print //

	UFUNCTION(BlueprintCallable)
	void SetHandType(EHandType Type);

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

	// Set Collision or Compoennt //

	void SetCombatCollisionEnabled();
	void SetCapsuleComponent(bool bIsActive);

	// Overlap //

	UFUNCTION()
	void OnCombatRightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCombatRightOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION() 
	void OnCombatLeftOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() 
	void OnCombatLeftOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
