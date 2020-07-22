// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerStatus.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

#include "MainPlayer.h"
#include "PlayerCombat.h"
#include "RPGSaveGame.h"

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

// Called when the game starts or when spawned
void APlayerStatus::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStatus(1);
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

void APlayerStatus::AdjustHP(float Amount, bool CanDie)
{
	Stat.CurHP += Amount;

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

float APlayerStatus::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (MainPlayer->bIsRoll)
	{
		return 0.f;
	}

	float total = DamageAmount - GetTotalDeffence();

	if (total > 0)
	{
		Stat.CurHP -= total;

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

	return total;
}

void APlayerStatus::Death()
{
	MainPlayer->SetMovementState(EMovementState::EMS_Dead);

	MainPlayer->PlayerCombat->PlayMontage(FName("Death"), 0.7f);
}

void APlayerStatus::DeathEnd()
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

void  APlayerStatus::SavePlayerStatData(URPGSaveGame* SaveGameInstance)
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

void  APlayerStatus::LoadPlayerStatData(URPGSaveGame* LoadGameInstance)
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