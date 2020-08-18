// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"

#include "Player/MainPlayer.h"
#include "Manager/GameManager.h"
#include "Manager/CombatManager.h"

#include "AIController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AttackTime = 0.1f;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (MonsterAIBP)
	{
		UE_LOG(LogTemp, Log, TEXT("Y!"));
		MonsterAI = GetWorld()->SpawnActor<AMonsterAI>(MonsterAIBP);

		if (MonsterAI)
		{
			MonsterAI->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	SetMonsterData();
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterState == EMonsterState::EMS_Death)
	{
		return;
	}

	if (CombatTarget && !bIsAttackAnim) // ���� ����� �ִ°�? and ������ �����°�?
	{
		AttackTarget(DeltaTime);
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

			GetCharacterMovement()->MaxWalkSpeed = Status.FollowSpeed;
		}
	}
}

void AMonster::SetMonsterState(EMonsterState State)
{
	MonsterState = State;

	if (MonsterState == EMonsterState::EMS_Follow)
	{
		GetCharacterMovement()->MaxWalkSpeed = Status.FollowSpeed;
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = Status.NormalSpeed;
	}
}


#pragma region Combat Function
void AMonster::AttackTarget(float Time)
{
	if (AttackCurTime == 0)
	{
		AttackTime = FMath::RandRange(AttackMinTime, AttackMaxTime);
	}
	
	AttackCurTime += Time;

	if (AttackCurTime >= AttackTime && CombatTarget && !bIsAttackAnim && !bIsAttack )
	{
		AttackCurTime = 0.f;
		AttackTime = 0.f;

		PlayAttackAnim();
	}
}

void AMonster::PlayAttackAnim()
{
	if (CombatTarget && !bIsAttackAnim && !bIsAttack)
	{
		bIsAttack = true;

		int32 Section = FMath::RandRange(0, 1);

		switch (Section)
		{
		case 0:
			PlayMontage(FName("Attack_1"), 1.3f);
			break;

		case 1:
			PlayMontage(FName("Attack_2"), 1.5f);
			break;

		default:
			break;
		}
	}
}

void AMonster::AttackStart()
{
	bIsAttackAnim = true;
}

void AMonster::AttackDamage()
{
	if (CombatTarget && DamageType)
	{
		UGameplayStatics::ApplyDamage(CombatTarget->PlayerStatus, Status.Damage, MonsterAI, this, DamageType);

		if (CombatTarget->GetMovementState() == EMovementState::EMS_Dead)
		{
			CombatTarget = nullptr;
			DetectTarget = nullptr;
		}
	}
}

void AMonster::AttackEnd()
{
	bIsAttackAnim = false;
	bIsAttack = false;

}

float AMonster::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
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
			/* ������� : #include "Kismet/GameplayStatics.h"

				���� ������ � ��ġ�� ��ƼŬ(Blood)�� ��½�Ű�� �ʹٸ�? ������ ���̷�Ż �޽��� XXX ������ ������ش�.
				�׸��� �Ʒ� �ڵ带 �ۼ��Ѵ�. �̶� SekeletalMeshSocket�� Include �������.

				const USkeletonMeshSocket* WeaponSocket = SekeltalMesh->GetSocketByName("XXXSocket");

				if(WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
				}

				�׸��� GetActorLocation() -> SocketLocation ���� �ٲ��ָ� �ȴ�.
			*/

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamagedParticle, GetActorLocation(), FRotator(0.f), false); // true : ������� �ڵ����� ��ƼŬ ����
		}

		if (DamagedSound)
		{
			UGameplayStatics::PlaySound2D(this, DamagedSound);
		}
	}
	return total;
}

void AMonster::Death()
{
	if (MonsterState != EMonsterState::EMS_Death)
	{
		SetMonsterState(EMonsterState::EMS_Death);

		GameManager->CombatManager->MonsterDeath(this);
		CombatTarget->RemoveWidgetMonster(this);
		CombatTarget = nullptr;
		DetectTarget = nullptr;

		PlayMontage(FName("Death"), 1.f);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMonster::DeathEnd()
{
	// ������Ʈ Ǯ�� �ٽ� �ǵ�����, ����� �׳� �ı���Ŵ

	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMonster::Respawn()
{

}
#pragma endregion

void AMonster::PlayMontage(FName Name, float PlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, PlayRate); // �ش� ��Ÿ�긦 n�� ������ ����Ѵ�.
		AnimInstance->Montage_JumpToSection(Name, CombatMontage);
	}
}
