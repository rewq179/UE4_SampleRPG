// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainPlayer.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Combat")
	TSubclassOf<UDamageType> DamageType;

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

	void PlayAttackAnim(); // ���� �ִϸ��̼�(��Ÿ��) ��� ����

	// �Ʒ��� �ִϸ��̼� Notify�� ������ �Լ���

	UFUNCTION(BlueprintCallable)
	void AttackStart(); // CombatCollision Ȱ��ȭ
	UFUNCTION(BlueprintCallable)
	void AttackDamage(); // ������ �ִ� ����
	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // CombatCollision ��Ȱ��ȭ

	void PlayMontage(FName Name, float PlayRate);

	float InterpSpeed;
	bool bIsInterp;

	FRotator GetTargetDirection(FVector Target);
	void InterpToMonster(float DeltaTime); // ĳ���Ͱ� ���ݴ���� �ٶ󺸰���

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
