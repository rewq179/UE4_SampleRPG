// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Monster.h"
#include "Monster/MonsterAI.h"
#include "Monster/MonsterPattern.h"

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

	CombatRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatRightCollision"));
	CombatRightCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightAttackSocket"));

	CombatLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatLeftCollision"));
	CombatLeftCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftAttackSocket"));
		
	bCanChargingAttack = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterAI = Cast<AMonsterAI>(GetController());
	
	CombatRightCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnCombatRightOverlapBegin);
	CombatRightCollision->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnCombatRightOverlapEnd);

	CombatLeftCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnCombatLeftOverlapBegin);
	CombatLeftCollision->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnCombatLeftOverlapEnd);

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
			Status.PatternID = (*MonsterTableRow).PatternID;
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
		if (MonsterPatternBP)
		{
			MonsterPattern = GetWorld()->SpawnActor<AMonsterPattern>(MonsterPatternBP);
			MonsterPattern->Monster = this;
			MonsterPattern->GameManager = GameManager;
			MonsterPattern->CombatManager = GameManager->CombatManager;

			ParseStringToInt(Status.PatternID);
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
		MonsterPattern->AddPattern(FCString::Atoi(*ID));
	}
}

void AMonster::AttackTarget(AMainPlayer* Target, EAttackClass AttackClass, int32 AttackNumber)
{
	if (Target)
	{
		CombatTarget = Target;
		if (MonsterPattern)
		{
			MonsterPattern->CombatTarget = Target;
		}

		if (CombatTarget)
		{
			FString SectionName = "Attack_";
			SectionName.Append(FString::FromInt(AttackNumber));

			UE_LOG(LogTemp, Log, TEXT("Section Name : %s"), *SectionName);
			int32 Index = AttackNumber - Status.AttackCount;

			switch (AttackClass)
			{
			case EAttackClass::EAC_Normal:
				SetAttackType(EAttackType::EAT_Normal);
				PlayMontage(FName(*SectionName), 1.5f);
				break;

			case EAttackClass::EAC_Charging:
				SetAttackType(EAttackType::EAT_KnockBack);

				PlayMontage(FName(*SectionName), 1.f);
				bCanChargingAttack = false;

				GetWorldTimerManager().SetTimer(TimeHandle, this, &AMonster::SetChargingAttack, 20.0f, false);
				break;

			case EAttackClass::EAC_Pattern:
				if (Status.bHasCharging)
				{
					Index--;
				}
				
				MonsterPattern->SelectedPatterns = MonsterPattern->Patterns[Index];
				PlayMontage(FName(*SectionName), 1.f);
				break;

			default:
				break;
			}
		}
	}

	else
	{
		CombatTarget = nullptr;
	}
}


void AMonster::TakeDamageHP(float DamageAmount, AActor* DamageCauser, EAttackType AttackType)
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

		CombatManager->ApplyDamageHP(CombatTarget->PlayerStatus, Status.Damage, this, AttackType, true, false);
		SetHandType(EHandType::EHT_None);

		if (CombatTarget->GetMovementState() == EMovementState::EMS_Dead)
		{
			CombatTarget = nullptr;
		}
	}
}

void AMonster::AttackAnimEnd()
{
	SetHandType(EHandType::EHT_None);

	OnAttackEnd.Broadcast();
}

void AMonster::SetHandType(EHandType Type)
{
	HandType = Type;

	SetCombatCollisionEnabled();
}

void AMonster::ChargingAnimEnd()
{
	SetHandType(EHandType::EHT_None);

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

void AMonster::OnCombatRightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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

void AMonster::OnCombatRightOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void AMonster::OnCombatLeftOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
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

void AMonster::OnCombatLeftOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void AMonster::SetCombatCollisionEnabled()
{
	switch (HandType)
	{
	case EHandType::EHT_None:
		CombatRightCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CombatLeftCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EHandType::EHT_Left:
		CombatRightCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CombatLeftCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;

	case EHandType::EHT_Right:
		CombatRightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CombatLeftCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EHandType::EHT_Both:
		CombatRightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CombatLeftCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;

	default:
		break;
	}

}
	
void AMonster::SetCapsuleComponent(bool bIsActive)
{
	if (bIsActive)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}

	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}