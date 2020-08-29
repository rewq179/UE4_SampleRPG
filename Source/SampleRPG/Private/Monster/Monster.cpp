// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Monster/MonsterSkill.h"

#include "Player/MainPlayer.h"
#include "Manager/GameManager.h"

#include "AIController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"



#include "TimerManager.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightAttackSocket"));
		
	bCanChargingAttack = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterAI = Cast<AMonsterAI>(GetController());
	
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnCombatOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnCombatOverlapEnd);

	SetMonsterData();
}

void AMonster::SetMonsterData()
{
	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		GameManager = Player->GameManager;
	}

	if (GameManager)
	{
		FMonsterTable* MonsterTableRow = GameManager->DataTableManager->GetMonsterTableData(MonsterID);

		if (MonsterTableRow)
		{
			Status.Name = (*MonsterTableRow).Name;

			Status.ID = (*MonsterTableRow).ID;
			Status.MonsterClass = (*MonsterTableRow).MonsterClass;
			Status.AttackCount = (*MonsterTableRow).AttackCount;
			Status.SkillID = (*MonsterTableRow).SkillID;
			Status.bHasCharging = (*MonsterTableRow).bHasCharging;
			Status.Name = (*MonsterTableRow).Name;
			Status.Level = (*MonsterTableRow).Level;
			Status.CurHP = (*MonsterTableRow).CurHP;
			Status.MaxHP = (*MonsterTableRow).MaxHP;
			Status.CurGroggy = (*MonsterTableRow).CurGroggy;
			Status.MaxGroggy = (*MonsterTableRow).MaxGroggy;
			Status.Damage = (*MonsterTableRow).Damage;
			Status.Deffence = (*MonsterTableRow).Deffence;
			Status.NormalSpeed = (*MonsterTableRow).NormalSpeed;
			Status.FollowSpeed = (*MonsterTableRow).FollowSpeed;
			Status.DetectRange = (*MonsterTableRow).DetectRange;
			Status.AttackRange = (*MonsterTableRow).AttackRange;
			Status.Exp = (*MonsterTableRow).Exp;	
			Status.Gold = (*MonsterTableRow).Gold;
			Status.ByProductID = (*MonsterTableRow).ByProductID;
			Status.RewardID = (*MonsterTableRow).RewardID;

			GetCharacterMovement()->MaxWalkSpeed = Status.NormalSpeed;
		}

		CombatManager = GameManager->CombatManager;
	}

	if (Status.MonsterClass == EMonsterClass::EMC_Boss)
	{
		if (MonsterSkillBP)
		{
			MonsterSkill = GetWorld()->SpawnActor<AMonsterSkill>(MonsterSkillBP);
			MonsterSkill->Monster = this;
			MonsterSkill->GameManager = GameManager;

			ParseStringToInt(Status.SkillID);
		}
	}
}

void AMonster::ParseStringToInt(FString Data)
{
	TArray<FString> StringID;

	FString DataString(Data);
	DataString.ParseIntoArray(StringID, TEXT("/"), true);

	for (auto ID : StringID)
	{
		MonsterSkill->AddSkill(FCString::Atoi(*ID));
	}
}

void AMonster::AttackTarget(AMainPlayer* Target, EAttackType AttackType)
{
	if (Target)
	{
		CombatTarget = Target;

		if (CombatTarget)
		{
			SetCombatCollisionEnabled(true);

			FString SectionName = "Attack_";
			SectionName.Append(FString::FromInt(GetAttackNumber(AttackType)));

			UE_LOG(LogTemp, Log, TEXT("Section Name : %s"), *SectionName);

			if (AttackType == EAttackType::EAT_Normal)
			{
				SetDamagedType(EDamagedType::EDT_Normal);
				
				PlayMontage(FName(*SectionName), 1.5f);
			}

			else
			{
				SetDamagedType(EDamagedType::EDT_KnockBack);

				PlayMontage(FName(*SectionName), 1.f);
				bCanChargingAttack = false;

				GetWorldTimerManager().SetTimer(TimeHandle, this, &AMonster::SetChargingAttack, 20.0f, false);
			}
		}
	}

	else
	{
		CombatTarget = nullptr;
	}
}

int32 AMonster::GetAttackNumber(EAttackType AttackType)
{
	switch (AttackType)
	{
	case EAttackType::EAT_Normal:
		return FMath::RandRange(0, Status.AttackCount -1 );
		break;

	case EAttackType::EAT_Charging:
		return FMath::RandRange(Status.AttackCount, Status.AttackCount);
		break;

	default:
		return -1;
	}
}

void AMonster::TakeDamage(float DamageAmount, AActor* DamageCauser, EDamagedType DamageType)
{
	float total = DamageAmount - Status.Deffence;

	if (total > 0)
	{
		Status.CurHP -= total;

		if (Status.CurHP <= 0)
		{
			Status.CurHP = 0;
			
			Death();
		}

		if (DamagedParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamagedParticle, GetActorLocation(), FRotator(0.f), false); // true : 재생이후 자동으로 파티클 삭제
		}

		if (DamagedSound)
		{
			UGameplayStatics::PlaySound2D(this, DamagedSound);
		}
	}
}

void AMonster::Death()
{
	MonsterAI->StopAI();

	GameManager->CombatManager->MonsterDeath(this);
	CombatTarget = nullptr;

	PlayMontage(FName("Death"), 1.f);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMonster::TakeGroggy(float DamageAmount, AActor* DamageCauser)
{
	Status.CurGroggy -= DamageAmount;

	if (Status.CurGroggy <= 0)
	{
		Status.CurGroggy = 0;

		Stun();
	}
}

void AMonster::Stun()
{

}

void AMonster::ApplyDamageToTarget()
{
	if (CombatTarget && bCanApplyDamage)
	{
		bCanApplyDamage = false;

		CombatManager->ApplyDamage(CombatTarget->PlayerStatus, Status.Damage, this, DamagedType, true);

		if (CombatTarget->GetMovementState() == EMovementState::EMS_Dead)
		{
			CombatTarget = nullptr;
		}
	}
}

void AMonster::AttackAnimEnd()
{
	SetCombatCollisionEnabled(false);

	OnAttackEnd.Broadcast();
}

void AMonster::ChargingAnimEnd()
{
	SetCombatCollisionEnabled(false);

	OnChargingEnd.Broadcast();
}

void AMonster::DeathAnimEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMonster::PlayMontage(FName Name, float PlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, PlayRate); // 해당 몽타쥬를 n배 빠르게 재상한다.
		AnimInstance->Montage_JumpToSection(Name, CombatMontage);
	}
}

void AMonster::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		auto Player = Cast<AMainPlayer>(OtherActor);

		if (Player && Player == CombatTarget)
		{
			bCanApplyDamage = true;
		}
	}
}

void AMonster::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		auto Player = Cast<AMainPlayer>(OtherActor);

		if (Player && Player == CombatTarget && Player->GetDistanceTo(this) > Status.AttackRange*1.5f)
		{
			bCanApplyDamage = false;
		}
	}
}

void AMonster::SetCombatCollisionEnabled(bool IsActive)
{
	if (IsActive)
	{
		CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	else
	{
		CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}