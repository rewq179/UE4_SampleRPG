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


// Sets default values
APlayerCombat::APlayerCombat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InterpSpeed = 15.f;
}

// Called when the game starts or when spawned
void APlayerCombat::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCombat::PlayAttackAnim()
{
	// 헤더파일 : #include "Engine/SkeletalMeshSocket.h"

	PlayMontage(FName("Attack_1"), 2.f);

	bIsInterp = true;
}

void APlayerCombat::ApplyDamageToTarget()
{
	if (TargetMonsters.Num() > 0)
	{
		TargetMonster = TargetMonsters[0];

		for (auto Monster : TargetMonsters)
		{
			CombatManager->ApplyDamageHP(Monster, MainPlayer->PlayerStatus->Stat.Damage, this, AttackType, false, false);
		}

		if (MainPlayer->Inventory->Equipments[0]->AttackSound)
		{
			UGameplayStatics::PlaySound2D(this, MainPlayer->Inventory->Equipments[0]->AttackSound);
		}
	}
}

void APlayerCombat::AttackAnimStart()
{
	MainPlayer->bIsAttackAnim = true;

	MainPlayer->Inventory->Equipments[0]->SetEnabledCombatCollision(true);
}

void APlayerCombat::AttackAnimEnd()
{
	MainPlayer->bIsAttackAnim = false;
	bIsInterp = false;

	MainPlayer->Inventory->Equipments[0]->SetEnabledCombatCollision(false);
	TargetMonsters.Empty();

}

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
			if (GetPriorityByClass(WidgetMonsters[MinIndex]->Status.MonsterClass) < GetPriorityByClass(WidgetMonsters[j]->Status.MonsterClass))
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

void APlayerCombat::PlayMontage(FName Name, float PlayRate)
{
	UAnimInstance* AnimInstance = MainPlayer->GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, PlayRate); // 해당 몽타쥬를 n배 빠르게 재상한다.
		AnimInstance->Montage_JumpToSection(Name, CombatMontage);
	}
}