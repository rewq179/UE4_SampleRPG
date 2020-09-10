// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCombat.h"
#include "Player/MainPlayer.h"
#include "Player/Inventory.h"
#include "Player/PlayerStatus.h"

#include "Item/Item.h"

#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

#include "TimerManager.h"

// Sets default values
APlayerCombat::APlayerCombat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComboCount = 0;
	DamageConst = 1.f;
	InterpSpeed = 15.f;
}

// Anim Blueprint & Montage //

void APlayerCombat::PlayAttackAnim(int32 AttackNumber, float PlayRate)
{
	FString SectionName = "Attack_";
	SectionName.Append(FString::FromInt(AttackNumber));

	PlayMontage(FName(*SectionName), PlayRate);
	GetWorldTimerManager().ClearTimer(TimerHandle);

	bIsInterp = true;

}


void APlayerCombat::SetComboValue(int32 DamageIndex)
{
	ComboLifeTime = 0.f;
	ComboCount = DamageIndex + 1;

	switch (DamageIndex)
	{
	case 0: // 구르기후 공격
		ComboCount = 1;
		DamageConst = 1.8f;
		break;

	case 1: // 1번 공격
		DamageConst = 1.0f;
		break;

	case 2: // 2번째 콤보
		DamageConst = 2.1f;
		break;

	case 3: // 마지막 
		ComboCount = 1;
		DamageConst = 3.5f;
		break;
	}

}

void APlayerCombat::CheckComboTimer()
{
	ComboLifeTime += GetWorldTimerManager().GetTimerElapsed(TimerHandle);

	if (ComboLifeTime > 1.f)
	{
		bCanComboAttack = false;
		ComboCount = 1;
		ComboLifeTime = 0.f;

		GetWorldTimerManager().ClearTimer(TimerHandle);

		return;
	}

	bCanComboAttack = true;
}

void APlayerCombat::AttackAnimStart()
{
	MainPlayer->bIsAttackAnim = true;

	Inventory->Equipments[0]->SetEnabledCombatCollision(true);
}

void APlayerCombat::ApplyDamageToTarget()
{
	if (TargetMonsters.Num() > 0)
	{
		TargetMonster = TargetMonsters[0];

		for (auto Monster : TargetMonsters)
		{
			CombatManager->ApplyDamageHP(Monster, PlayerStatus->GetTotalDamage() * DamageConst, this, AttackType, false, false);
		}

		if (Inventory->Equipments[0]->AttackSound)
		{
			UGameplayStatics::PlaySound2D(this, Inventory->Equipments[0]->AttackSound);
		}
	}
}

void APlayerCombat::AttackAnimEnd()
{
	MainPlayer->bIsAttackAnim = false;
	bIsInterp = false;

	Inventory->Equipments[0]->SetEnabledCombatCollision(false);
	TargetMonsters.Empty();

	if (!MainPlayer->bIsRoll)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCombat::CheckComboTimer, 0.1f, true, 0.f);
	}
}

void  APlayerCombat::PlayMontage(FName AnimName, float PlayRate)
{
	UAnimInstance* AnimInstance = MainPlayer->GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, PlayRate); // 해당 몽타쥬를 n배 빠르게 재상한다.
		AnimInstance->Montage_JumpToSection(AnimName, CombatMontage);
	}
}

// HUD : Monster Info(Boss > Elite > Normal) //

void APlayerCombat::UpdateWidgetMonster()
{
	if (WidgetMonsters.Num() == 0)
	{
		TargetMonster = nullptr;

		return;
	}

	for (int32 i = 0; i < WidgetMonsters.Num() - 1; i++)
	{
		int32 MinIndex = i;

		for (int32 j = i + 1; j < WidgetMonsters.Num(); j++)
		{
			UE_LOG(LogTemp, Log, TEXT("Pri : %d"), (int32)WidgetMonsters[MinIndex]->Status.MonsterClass);

			if ((int32)WidgetMonsters[MinIndex]->Status.MonsterClass < (int32)WidgetMonsters[j]->Status.MonsterClass)
			{
				MinIndex = j;
			}
		}

		WidgetMonsters.Swap(i, MinIndex);
	}

	TargetMonster = WidgetMonsters[0];
}

int32 APlayerCombat::GetPriorityByClass(EMonsterClass Class)
{
	switch (Class)
	{
	case EMonsterClass::EMC_Normal:
		return 1;

	case EMonsterClass::EMC_Elite:
		return 2;

	case EMonsterClass::EMC_Boss:
		return 3;

	default:
		return -1;
	}
}

// Player Interp //

void APlayerCombat::InterpToMonster(float DeltaTime)
{
	if (bIsInterp && TargetMonsters.Num() > 0)
	{
		FRotator DirectionYaw = GetTargetDirection(TargetMonsters[0]->GetActorLocation());
		FRotator InterpRotaion = FMath::RInterpTo(GetActorRotation(), DirectionYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotaion);
	}
}

FRotator APlayerCombat::GetTargetDirection(FVector Target)
{
	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator DirectionYaw(0.f, Direction.Yaw, 0.f);

	return DirectionYaw;
}

