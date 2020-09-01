// Fill out your copyright notice in the Description page of Project Settings.

#include "Pattern.h"

#include "GameManager.h"
#include "MainPlayer.h"
#include "Monster.h"
#include "MonsterPattern.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APattern::APattern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(GetRootComponent());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());

	UseParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("UseParticle"));
	UseParticle->SetupAttachment(GetRootComponent());
	UseParticle->bAutoActivate = false;

	NotifyFieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("NotifyFieldParticle"));
	NotifyFieldParticle->SetupAttachment(GetRootComponent());
	NotifyFieldParticle->bAutoActivate = false;

	DamageFieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamageFieldParticle"));
	DamageFieldParticle->SetupAttachment(GetRootComponent());
	DamageFieldParticle->bAutoActivate = false;
}

// Called when the game starts or when spawned
void APattern::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APattern::OnBoxOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APattern::OnBoxOverlapEnd);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APattern::OnSphereOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APattern::OnSphereOverlapEnd);
	
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsNotifyFieldActive && PatternData.PatternClass == EPatternClass::EPT_Teleport)
	{
		SetTargetLocationByGroundSurface();

		NotifyFieldParticle->SetRelativeLocation(TargetLocation);

		if (!NotifyFieldParticle->IsActive())
		{
			NotifyFieldParticle->ToggleActive();
		}
	}
}


void APattern::SetCollisionSize()
{
	switch (PatternData.PatternShape.Shape)
	{
	case EShape::ES_Circle:
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereCollision->SetSphereRadius(PatternData.PatternShape.Size.X);
		break;

	case EShape::ES_Rectangle:
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxCollision->SetBoxExtent(PatternData.PatternShape.Size);
		break;

	default:
		break;
	}
}

void APattern::SetActiveCollision(bool bIsActive)
{
	switch (PatternData.PatternShape.Shape)
	{
	case EShape::ES_Circle:
		if (bIsActive)
		{
			SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}

		else
		{
			SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	case EShape::ES_Rectangle:
		if (bIsActive)
		{
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}

		else
		{
			BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	default:
		break;
	}
}

void APattern::PlayUseParticle()
{
	UseParticle->SetRelativeLocation(Monster->GetMesh()->GetSocketLocation("UsePattern"));
	UseParticle->ToggleActive();
}

void APattern::PlayNotifyParticle(bool bIsTargetLocation)
{
	if (bIsTargetLocation) // 타겟 위치에 발동되는 패턴
	{
		if (PatternData.PatternClass == EPatternClass::EPT_Teleport)
		{
			bIsNotifyFieldActive = true;

			return;
		}

		SetTargetLocationByGroundSurface();

		NotifyFieldParticle->SetRelativeLocation(TargetLocation);
		NotifyFieldParticle->ToggleActive();
	}

	else // 랜덤 위치에 발동되는 패턴
	{
		SetRandLocationByGroundSurface();

		NotifyFieldParticle->SetRelativeLocation(RandLocation);
		NotifyFieldParticle->ToggleActive();
	}
}

void APattern::PlayDamageParticle(bool bIsTargetLocation)
{
	SetActiveCollision(true);

	if (bIsTargetLocation)
	{
		bIsNotifyFieldActive = false;

		if (PatternData.PatternClass == EPatternClass::EPT_Teleport)
		{
			Monster->SetActorLocation(TargetLocation);
		}

		DamageFieldParticle->SetRelativeLocation(TargetLocation);
		DamageFieldParticle->ToggleActive();
	}

	else
	{
		DamageFieldParticle->SetRelativeLocation(RandLocation);
		DamageFieldParticle->ToggleActive();
	}
}


void APattern::SetTargetLocationByGroundSurface()
{
	FHitResult Hit;

	FVector Start = Monster->CombatTarget->GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 500.f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Monster->CombatTarget);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		if (Hit.bBlockingHit)
		{
			TargetLocation = Hit.ImpactPoint;
		}
	}

	SetActorLocation(TargetLocation);
}

void APattern::SetRandLocationByGroundSurface()
{
	SetTargetLocationByGroundSurface();

	FVector Extent = BoxComponent->GetScaledBoxExtent();
	FVector Origin = BoxComponent->GetComponentLocation();

	RandLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	RandLocation.Z = TargetLocation.Z;

	SetActorLocation(RandLocation);
}



void APattern::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		Target = Cast<AMainPlayer>(OtherActor);

		if (Target)
		{
			Monster->MonsterPattern->ApplyPatternDamageToTarget();

			SetActiveCollision(false);
		}
	}
}


void APattern::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		Target = nullptr;
	}
}

void APattern::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		Target = Cast<AMainPlayer>(OtherActor);
	
		if (Target)
		{
			Monster->MonsterPattern->ApplyPatternDamageToTarget();

			SetActiveCollision(false);
		}
	}
}


void APattern::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == Target)
	{
		Target = nullptr;
	}
}
