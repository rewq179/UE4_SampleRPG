// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "MainPlayer.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(GetRootComponent());
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/MonsterTable.MonsterTable'"));

	if (DataTable.Succeeded())
	{
		MonsterTable = DataTable.Object;
	}
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnDetectOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnDetectOverlapEnd);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnCombatOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnCombatOverlapEnd);

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
		PlayAttackAnim();
	}

	if(!CombatTarget && DetectTarget)
	{
		FollowTarget();
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::SetMonsterData()
{
	if (MonsterTable)
	{
		FMonsterTable* MonsterTableRow = MonsterTable->FindRow<FMonsterTable>(FName(*(FString::FormatAsNumber(MonsterID))), FString(""));

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

			DetectSphere->SetSphereRadius(Status.DetectRange);
			CombatSphere->SetSphereRadius(Status.AttackRange);

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

#pragma region Overlap Begin/End

void  AMonster::OnDetectOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && !DetectTarget)
	{
		DetectTarget = Cast<AMainPlayer>(OtherActor);
	}
}

void  AMonster::OnDetectOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsTargetPlayer(OtherActor) && !CombatTarget)
	{
		DetectTarget = nullptr;
		bIsAttackAnim = false;

		SetMonsterState(EMonsterState::EMS_Idle);

		AIController->StopMovement(); // �������� ����� �����.
	}
}

void  AMonster::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsTargetPlayer(OtherActor))
	{
		CombatTarget = Cast<AMainPlayer>(OtherActor);

		SetMonsterState(EMonsterState::EMS_Combat);

		bCanAttack = true;
	}
}

void  AMonster::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsTargetPlayer(OtherActor))
	{
		CombatTarget = nullptr;

		SetMonsterState(EMonsterState::EMS_Follow);

		bCanAttack = false;
	}
}

bool AMonster::IsTargetPlayer(AActor* OtherActor)
{
	if (OtherActor)
	{
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
	
		if (Player == DetectTarget || Player == CombatTarget)
		{
			return true;
		}
	}

	return false;
}

#pragma endregion

void AMonster::FollowTarget()
{
	if (AIController && DetectTarget && !bIsAttackAnim)
	{
		SetMonsterState(EMonsterState::EMS_Follow);
	
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(DetectTarget); // ��ǥ�� : Ÿ��
		MoveRequest.SetAcceptanceRadius(10.f); // ������ ������ ���ΰ�?

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

#pragma region Combat Function

void AMonster::PlayAttackAnim()
{
	if (CombatTarget && !bIsAttackAnim && !bIsAttack)
	{
		bIsAttack = true;

		int32 Section = FMath::RandRange(0, 1);
		UE_LOG(LogTemp, Log, TEXT("Num = %d"), Section);

		switch (Section)
		{
		case 0:
			PlayMontage(FName("Attack_1"), 1.2f);
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
	if (CombatTarget)
	{
		CombatTarget->TakeDamage(Status.Damage);
	}
}

void AMonster::AttackEnd()
{
	bIsAttackAnim = false;
	bIsAttack = false;
}

void AMonster::TakeDamage(float Damage)
{
	int total = Damage - Status.Deffence;

	if (total > 0)
	{
		Status.CurHP -= total;

		if (Status.CurHP <= 0)
		{
			Death();
		}
	}

	if (DamagedParticle)
	{
		// ������� : #include "Kismet/GameplayStatics.h"

		/*
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

void AMonster::Death()
{
	SetMonsterState(EMonsterState::EMS_Death);

	CombatTarget->AddExp(Status.Exp);
	CombatTarget->AddGold(Status.Gold);

	PlayMontage(FName("Death"), 0.5f);
	
}

void AMonster::ReturnMonster()
{
	// ������Ʈ Ǯ�� �ٽ� �ǵ�����, ����� �׳� �ı���Ŵ

	Destroy();
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
