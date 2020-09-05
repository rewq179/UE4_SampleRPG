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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Conponents")
	class AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStatus|Properties")
	class USoundCue* DamagedSound;

	FTimerHandle TimeHandle;

	class UDataTable* PlayerStatTable;
	FPlayerStatTable* PlayerStatTableRow;

	// 스탯 //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Stat")
	float StaminaRate; // 스테미나 증가량

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

	// 스킬 //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Skill")
	TMap<int32, FSkillTable> SkillMaps; // 버프와 디버프 모음집

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Skill")
	FSkillTable RecentSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStatus|Skill")
	float LifeTime; // 스킬의 지속시간
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 스탯 //

	void IncreaseStamina(float DeltaTime);

	void AddExp(int32 Exp);
	void CheckLevelUP();
	void SetLevelStatus(int32 CurLevel);

	void AdjustHP(float DamageAmount, bool CanDie);

	void TakeDamageHP(float DamageAmount, AActor* DamageCauser, EAttackType AttackType);
	void TakeDamageST(float DamageAmount);
	
	// 애니메이션 //

	UFUNCTION(BlueprintCallable)
	void KnockDownAnimEnd();

	void Death();

	UFUNCTION(BlueprintCallable)
	void DeathAnimEnd();

	void Revive();

	// 세이브 로드 //

	void SavePlayerStatData(class USaveGameManager* SaveGameInstance);
	void LoadPlayerStatData(class USaveGameManager* LoadGameInstance);
	
	// 상태이상 //

	void SetDebuffToPlayer(EAttackType AttackType);
	
	// HUD  //

	UFUNCTION(BlueprintImplementableEvent)
	void SetSystemText();
};
