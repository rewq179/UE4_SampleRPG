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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Properties")
	class USphereComponent* DetectColiision; // ���� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Properties")
	class USphereComponent* CombatColiision; // ���� ����

	/* 
		���� Skeleton �޽��� Socket�� �����ϰ�, �װ��� Compnent*�� �ٿ��� ����� �� �� ����.

		�׷���� CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("���� ��Ī"))

		�׸��� CombatColiision�� ���� ���� ���� ���ľ��Ѵ�.

		CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ���� ���۽� Active���ָ��.
		CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		CombatCollision->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
		CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		���������� ��ƼŬ�� ������ ��ġ���� �����ϸ� ��!
	*/

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	float AttackMaxTime;

	/*
		���� Tick�� �̿����� �ʴ´ٸ� FTimerHandle�� �̿��ؾ��Ѵ�.

		GetWorldTimerManager().SetTimer(�ð��ڵ�, this, ���� �Լ� ��Ī, Ÿ�̸� �ð�)��
		GetWorldTimerManager().ClearTimer(Ÿ�̸� �ð�)�� �̿��ϳ�.
	*/

	float AttackCurTime;
	float AttackTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bCanAttack; // �÷��̾ ����(���� ��Ÿ� IN??)�� �� �ִ°�?

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bIsAttack; // ��Ÿ�� �˻��� 1ȸ�ϱ� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Combat")
	bool bIsAttackAnim; // ���� �ִϸ��̼��� �����°�?
	
	void AttackTarget(float Time);
	void PlayAttackAnim();

	UFUNCTION(BlueprintCallable)
	void AttackStart(); // bIsAttacking = true

	UFUNCTION(BlueprintCallable)
	void AttackDamage(); // ������ �ִ� ����

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

	UFUNCTION() // ���� �ڽ� Ŭ�������� �������ϸ� UFUNCTION()�� �����ؾ��Ѵ�.
	virtual void OnDetectOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnDetectOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() // ���� �ڽ� Ŭ�������� �������ϸ� UFUNCTION()�� �����ؾ��Ѵ�.
	virtual void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	void FollowTarget();
	void PlayMontage(FName Name, float PlayRate);
};
