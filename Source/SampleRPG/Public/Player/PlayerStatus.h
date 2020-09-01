// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "Manager/CombatManager.h"
#include "Manager/DataTableManager.h"

#include "PlayerStatus.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStatTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		int32 CurExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		int32 MaxExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		float CurStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		float Deffence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		int32 Dexterity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
		int32 Intelligence;
};

UCLASS()
class SAMPLERPG_API APlayerStatus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerStatus();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class ASkillManager* SkillManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class USoundCue* DamagedSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|Properties")
	float StaminaRate; // 스테미나 증가량

	FTimerHandle TimeHandle;

	class UDataTable* PlayerStatTable;
	FPlayerStatTable* PlayerStatTableRow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Stat")
	FPlayerStatTable Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Stat")
	float IncDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Stat")
	float IncDeffence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Stat")
	int32 IncStrength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Stat")
	int32 IncDexterity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Stat")
	int32 IncIntelligence;


	FORCEINLINE float GetTotalDamage() { return IncDamage + Stat.Damage; }
	FORCEINLINE float GetTotalDeffence() { return IncDeffence + Stat.Deffence; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Stat")
	bool bIsPlayerDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|Skill")
	TMap<int32, FSkillTable> SkillMaps;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|Skill")
	FSkillTable RecentSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|Skill")
	float LifeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Skill")
	float OriginSpeed;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void IncreaseStamina(float DeltaTime);

	void AdjustHP(float Amount, bool CanDie);

	void TakeDamageHP(float DamageAmount, AActor* DamageCauser, EAttackType AttackType);
	void TakeDamageST(float DamageAmount, AActor* DamageCauser, EAttackType DamageType);
	void SetPlayerCrowdControl(EAttackType AttackType);
	
	UFUNCTION(BlueprintCallable)
	void KnockDownAnimEnd();

	void Death();

	UFUNCTION(BlueprintCallable)
	void DeathAnimEnd();

	void Revive();

	void AddExp(int32 Exp);
	void CheckLevelUP();
	void SetLevelStatus(int32 CurLevel);

	void SavePlayerStatData(class USaveGameManager* SaveGameInstance);
	void LoadPlayerStatData(class USaveGameManager* LoadGameInstance);
	
	void SetPoison();
	void SetFrostbite();
	void ResetStatus();

	UFUNCTION(BlueprintImplementableEvent)
	void SetSystemText();
};
