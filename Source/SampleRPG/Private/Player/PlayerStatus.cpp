// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerStatus.h"
#include "Player/PlayerCombat.h"
#include "Player/MainPlayer.h"

#include "Manager/SaveGameManager.h"
#include "Manager/SkillManager.h"

#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"

#include "TimerManager.h"

// Sets default values
APlayerStatus::APlayerStatus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/PlayerStatTable.PlayerStatTable'"));

	if (DataTable.Succeeded())
	{
		PlayerStatTable = DataTable.Object;
	}

	StaminaRate = 15.f;
}


// Called every frame
void APlayerStatus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IncreaseStamina(DeltaTime);
}

void APlayerStatus::IncreaseStamina(float DeltaTime)
{
	if (Stat.CurStamina < Stat.MaxStamina)
	{
		Stat.CurStamina += DeltaTime * StaminaRate;
	}

	else
	{
		Stat.CurStamina = Stat.MaxStamina;
	}
}

void APlayerStatus::AddExp(int32 Exp)
{
	Stat.CurExp += Exp;

	CheckLevelUP();
}

void APlayerStatus::CheckLevelUP()
{
	while (Stat.CurExp >= Stat.MaxExp)
	{
		int32 Exp = Stat.CurExp - Stat.MaxExp;

		SetLevelStatus(Stat.Level + 1);

		Stat.CurExp = Exp;
	}
}

void APlayerStatus::SetLevelStatus(int32 CurLevel)
{
	if (PlayerStatTable)
	{
		PlayerStatTableRow = PlayerStatTable->FindRow<FPlayerStatTable>(FName(*(FString::FormatAsNumber(CurLevel - 1))), FString(""));

		if (PlayerStatTableRow)
		{
			Stat.Level = (*PlayerStatTableRow).Level;
			Stat.MaxExp = (*PlayerStatTableRow).MaxExp;
			Stat.CurExp = (*PlayerStatTableRow).CurExp;
			Stat.MaxHP = (*PlayerStatTableRow).MaxHP;
			Stat.CurHP = (*PlayerStatTableRow).CurHP;
			Stat.MaxStamina = (*PlayerStatTableRow).MaxStamina;
			Stat.CurStamina = (*PlayerStatTableRow).CurStamina;
			Stat.Damage = (*PlayerStatTableRow).Damage;
			Stat.Deffence = (*PlayerStatTableRow).Deffence;
			Stat.Strength = (*PlayerStatTableRow).Strength;
			Stat.Dexterity = (*PlayerStatTableRow).Dexterity;
			Stat.Intelligence = (*PlayerStatTableRow).Intelligence;
		}
	}
}

void APlayerStatus::AdjustHP(float DamageAmount, bool CanDie) 
{
	// 상태이상이나 함정을 밟았을때 방어력 무시로 들어가도록 만들어놓은 함수
	
	Stat.CurHP += DamageAmount;

	if (Stat.CurHP > Stat.MaxHP)
	{
		Stat.CurHP = Stat.MaxHP;
	}

	if (Stat.CurHP <= 0)
	{
		if (CanDie)
		{
			Death();
		}

		else
		{
			Stat.CurHP = 1.f;
		}
	}
}

void APlayerStatus::TakeDamageHP(float DamageAmount, AActor* DamageCauser, EAttackType AttackType)
{
	if (MainPlayer->bIsRoll)
	{
		return;
	}

	SetDebuffToPlayer(AttackType);

	float total = DamageAmount - GetTotalDeffence();

	if (total > 0)
	{
		Stat.CurHP -= total;
	}

	else
	{
		Stat.CurHP--;
	}

	if (Stat.CurHP <= 0)
	{
		Stat.CurHP = 0;
		Death();
	}

	if (DamagedSound)
	{
		UGameplayStatics::PlaySound2D(this, DamagedSound, 0.5f);
	}
}

void APlayerStatus::TakeDamageST(float DamageAmount)
{
	if (MainPlayer->bIsRoll)
	{
		return;
	}

	Stat.CurStamina -= DamageAmount;

	if (Stat.CurStamina <= 0)
	{
		Stat.CurStamina = 0;
	}
}

void APlayerStatus::SetDebuffToPlayer(EAttackType AttackType)
{
	if (AttackType == EAttackType::EAT_Poison || AttackType == EAttackType::EAT_Frostbite)
	{
		MainPlayer->PlayerCombat->CombatManager->SetDebuffToPlayer(AttackType);

		SetSystemText();
	}
}

void APlayerStatus::KnockDownAnimEnd()
{
	MainPlayer->bCanMove = true;
}

void APlayerStatus::Death()
{
	MainPlayer->bIsDead = true;

	MainPlayer->PlayerCombat->PlayMontage(FName("Death"), 0.7f);
}

void APlayerStatus::DeathAnimEnd()
{
	MainPlayer->GetMesh()->bPauseAnims = true;
	MainPlayer->GetMesh()->bNoSkeletonUpdate = true;
}

void APlayerStatus::Revive()
{
	Stat.CurHP = Stat.MaxHP;

	MainPlayer->GetMesh()->bPauseAnims = false;
	MainPlayer->GetMesh()->bNoSkeletonUpdate = false;
}

void  APlayerStatus::SavePlayerStatData(USaveGameManager* SaveGameInstance)
{
	SaveGameInstance->PlayerStat.Level = Stat.Level;
	SaveGameInstance->PlayerStat.CurExp = Stat.CurExp;
	SaveGameInstance->PlayerStat.MaxExp = Stat.MaxExp;
	SaveGameInstance->PlayerStat.CurHP = Stat.CurHP;
	SaveGameInstance->PlayerStat.MaxHP = Stat.MaxHP;
	SaveGameInstance->PlayerStat.CurStamina = Stat.CurStamina;
	SaveGameInstance->PlayerStat.MaxStamina = Stat.MaxStamina;
	SaveGameInstance->PlayerStat.Damage = Stat.Damage;
	SaveGameInstance->PlayerStat.Deffence = Stat.Deffence;
	SaveGameInstance->PlayerStat.Strength = Stat.Strength;
	SaveGameInstance->PlayerStat.Dexterity = Stat.Dexterity;
	SaveGameInstance->PlayerStat.Intelligence = Stat.Intelligence;
}

void  APlayerStatus::LoadPlayerStatData(USaveGameManager* LoadGameInstance)
{
	Stat.Level = LoadGameInstance->PlayerStat.Level;
	Stat.CurExp = LoadGameInstance->PlayerStat.CurExp;
	Stat.MaxExp = LoadGameInstance->PlayerStat.MaxExp;
	Stat.CurHP = LoadGameInstance->PlayerStat.CurHP;
	Stat.MaxHP = LoadGameInstance->PlayerStat.MaxHP;
	Stat.CurStamina = LoadGameInstance->PlayerStat.CurStamina;
	Stat.MaxStamina = LoadGameInstance->PlayerStat.MaxStamina;
	Stat.Damage = LoadGameInstance->PlayerStat.Damage;
	Stat.Deffence = LoadGameInstance->PlayerStat.Deffence;
	Stat.Strength = LoadGameInstance->PlayerStat.Strength;
	Stat.Dexterity = LoadGameInstance->PlayerStat.Dexterity;
	Stat.Intelligence = LoadGameInstance->PlayerStat.Intelligence;
}