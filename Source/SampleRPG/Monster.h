// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Monster.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Patrol UMETA(DisplayName = "Patrol"),
	EMS_Follow UMETA(DisplayName = "Follow"),
	EMS_Combat UMETA(DisplayName = "Combat"),
	EMS_Death UMETA(DisplayName = "Death"),

	EMS_MAX
};

UENUM(BlueprintType)
enum class EMonsterClass : uint8
{
	EMC_Normal UMETA(DisplayName = "Normal"),
	EMC_Elite UMETA(DisplayName = "Elite"),
	EMC_Boss UMETA(DisplayName = "Boss"),

	EMC_MAX
};

USTRUCT(BlueprintType)
struct FMonsterTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	EMonsterClass MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float CurGroggy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float MaxGroggy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float Deffence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float FollowSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float DetectRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	int32 Exp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
	int32 Gold;

};

UCLASS()
class SAMPLERPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

#pragma region MonsterTable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|MonsterTable")
	int32 MonsterID;

	class UDataTable* MonsterTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|MonsterTable")
	FMonsterTable Status;

	void SetMonsterData();
#pragma endregion

#pragma region Properties (Components, Particle, Sound, etc...)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|State")
	EMonsterState MonsterState;

	FORCEINLINE void SetMonsterState(EMonsterState State);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Properties")
	class USphereComponent* DetectSphere; // ���� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Properties")
	class USphereComponent* CombatSphere; // ���� ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UParticleSystem* DamagedParticle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class USoundCue* DamagedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster|Properties")
	class UAnimMontage* CombatMontage;


#pragma endregion

#pragma region Combat And AI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AMainPlayer* DetectTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	class AMainPlayer* CombatTarget;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bCanAttack; // �÷��̾ ����(���� ��Ÿ� IN??)�� �� �ִ°�?

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bIsAttack; // ��Ÿ�� �˻��� 1ȸ�ϱ� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bIsAttackAnim; // ���� �ִϸ��̼��� �����°�?
	
	void PlayAttackAnim();

	UFUNCTION(BlueprintCallable)
	void AttackStart(); // bIsAttacking = true

	UFUNCTION(BlueprintCallable)
	void AttackDamage(); // ������ �ִ� ����

	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // bIsAttacking = false

	UFUNCTION(BlueprintCallable)
	void ReturnMonster();
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() // ���� �ڽ� Ŭ�������� �������ϸ� UFUNCTION()�� �����ؾ��Ѵ�.
	virtual void OnDetectOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnDetectOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() // ���� �ڽ� Ŭ�������� �������ϸ� UFUNCTION()�� �����ؾ��Ѵ�.
	virtual void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void TakeDamage(float Damage);
	void Death();
	void FollowTarget();
	void PlayMontage(FName Name, float PlayRate);

	bool IsTargetPlayer(AActor* OtherActor);
};
