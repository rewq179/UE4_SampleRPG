// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "Manager/CombatManager.h"
#include "Manager/DataTableManager.h"

#include "PlayerStatus.generated.h"

/**
 * �÷��̾��� �������ͽ��� ���� �Լ��� �������� ��Ƴ���.
*/

USTRUCT(BlueprintType)
struct FPlayerStatTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		int32 CurExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		int32 MaxExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		float CurStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		float Deffence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		int32 Dexterity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat|Status")
		int32 Intelligence;
};

UCLASS()
class SAMPLERPG_API APlayerStatus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerStatus();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|ManagerClass")
	class ASkillManager* SkillManager;

	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Conponents")
	class AMainPlayer* MainPlayer;

	// Properties //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStatus|Properties")
	class USoundCue* DamagedSound;

	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Timer")
	FItemTable HoldPotionData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Timer")
	float LifeTime;

	// DataTable //

	class UDataTable* PlayerStatTable;
	FPlayerStatTable* PlayerStatTableRow;

	// Player Stat //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	float StaminaRate; // ���׹̳� ������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	float IncStaminaRate; // ���¹̳� �߰� ȸ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	FPlayerStatTable Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	float IncDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	float IncDeffence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	int32 IncStrength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	int32 IncDexterity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	int32 IncIntelligence;
	
	FORCEINLINE float GetTotalDamage() { return IncDamage + Stat.Damage; }
	FORCEINLINE float GetTotalDeffence() { return IncDeffence + Stat.Deffence; }

	// Buff & Debuff Skill //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Skill")
	TMap<int32, FSkillTable> SkillMaps; // ������ ����� ������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Skill")
	FSkillTable RecentSkill;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; // ���¹̳� �ʴ� ȸ���� �����

	// Tick Player Stat //

	void IncreaseStamina(float DeltaTime);

	void SetRecoveryStat(FItemTable ItemData);
	void RecoveryHP();
	void RecoveryST();

	bool IsLifeTimeOver(float DurationTime);

	// Player Stat //

	void SetLevelStatus(int32 CurLevel);

	void AddExp(int32 Exp);
	void CheckLevelUP();

	// Combat //

	void AdjustHP(float DamageAmount, bool CanDie);
	void AdjustST(float DamageAmount);

	void TakeDamageHP(float DamageAmount, AActor* DamageCauser, EAttackType AttackType);
	void TakeDamageST(float DamageAmount);
	
	void Death();
	void Revive();

	// Buff & Debuff Skill //

	void SetDebuffToPlayer(EAttackType AttackType);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetSystemText();

	// Anim Blueprint & Montage //

	UFUNCTION(BlueprintCallable)
	void KnockDownAnimEnd();

	UFUNCTION(BlueprintCallable)
	void DeathAnimEnd();


	// Save & Load GameData //

	void SavePlayerStatData(class USaveGameManager* SaveGameInstance);
	void LoadPlayerStatData(class USaveGameManager* LoadGameInstance);
	
};
