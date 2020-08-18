// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/DataTableManager.h"

#include "Monster.generated.h"

UCLASS()
class SAMPLERPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditDefaultsOnly, Category = "Monster|BehaviorTree")
	TSubclassOf<class AMonsterAI> MonsterAIBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|BehaviorTree")
	class AMonsterAI* MonsterAI;

#pragma region MonsterTable

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|MonsterTable")
	int32 MonsterID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|MonsterTable")
	FMonsterTable Status;

	void SetMonsterData();
#pragma endregion

#pragma region Properties (Components, Particle, Sound, etc...)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|State")
	EMonsterState MonsterState;

	FORCEINLINE void SetMonsterState(EMonsterState State);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UParticleSystem* DamagedParticle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class USoundCue* DamagedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UAnimMontage* CombatMontage;


#pragma endregion

#pragma region Combat And AI

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AMainPlayer* DetectTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AMainPlayer* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	float AttackMaxTime;

	/*
		만약 Tick을 이용하지 않는다면 FTimerHandle을 이용해야한다.

		GetWorldTimerManager().SetTimer(시간핸들, this, 실행 함수 명칭, 타이머 시간)과
		GetWorldTimerManager().ClearTimer(타이머 시간)을 이용하낟.
	*/

	float AttackCurTime;
	float AttackTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bCanAttack; // 플레이어를 공격(공격 사거리 IN??)할 수 있는가?

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bIsAttack; // 몽타쥬 검색을 1회하기 위함

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bIsAttackAnim; // 공격 애니메이션이 종료됬는가?
	
	void AttackTarget(float Time);
	void PlayAttackAnim();

	UFUNCTION(BlueprintCallable)
	void AttackStart(); // bIsAttacking = true

	UFUNCTION(BlueprintCallable)
	void AttackDamage(); // 데미지 주는 시점

	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // bIsAttacking = false

	void Death();
	void Respawn();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Monster|Combat")
	TSubclassOf<UDamageType> DamageType;
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	
	void PlayMontage(FName Name, float PlayRate);
};
