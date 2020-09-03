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

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(GetRootComponent());


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

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APattern::OnSphereOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APattern::OnSphereOverlapEnd);
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsNotifyFieldActive && PatternData.PatternClass == EPatternClass::EPT_Teleport)
	{
		SetTargetLocationByGroundSurface();

		NotifyFieldParticle->SetRelativeLocation(TargetLocation);
	}
}


void APattern::SetCollisionSize()
{
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetSphereRadius(PatternData.PatternShape.Size.X);
}

void APattern::SetActiveCollision(bool bIsActive)
{
	if (bIsActive)
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	else
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APattern::PlayUseParticle()
{
	UseParticle->ToggleActive();
	UseParticle->SetRelativeLocation(Monster->GetMesh()->GetSocketLocation("UsePattern"));
}

void APattern::PlayNotifyParticle(bool bIsTargetLocation)
{
	NotifyFieldParticle->ToggleActive();

	if (bIsTargetLocation) // 타겟 위치에 발동되는 패턴
	{
		if (PatternData.PatternClass == EPatternClass::EPT_Teleport)
		{
			bIsNotifyFieldActive = true;
			Monster->SetCapsuleComponent(false);

			return;
		}

		SetTargetLocationByGroundSurface();
		
		NotifyFieldParticle->SetRelativeLocation(TargetLocation);
	}

	else // 랜덤 위치에 발동되는 패턴
	{
		SetRandLocationByGroundSurface();

		NotifyFieldParticle->SetRelativeLocation(RandLocation);
	}
}

void APattern::PlayDamageParticle(bool bIsTargetLocation)
{
	SetActiveCollision(true);
	DamageFieldParticle->ToggleActive();

	if (bIsTargetLocation)
	{
		if (PatternData.PatternClass == EPatternClass::EPT_Teleport)
		{
			bIsNotifyFieldActive = false;

			Monster->SetActorLocation(TargetLocation);
		}

		DamageFieldParticle->SetRelativeLocation(TargetLocation);
	}

	else
	{
		DamageFieldParticle->SetRelativeLocation(RandLocation);
	}
}


void APattern::SetTargetLocationByGroundSurface()
{
	FHitResult Hit;

	FVector Start = Monster->CombatTarget->GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 1000.f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Monster->CombatTarget);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		if (Hit.bBlockingHit)
		{
			TargetLocation = Hit.ImpactPoint;
	
			SetCollisionLocation(TargetLocation);
		}
	}

}

void APattern::SetRandLocationByGroundSurface()
{
	SetTargetLocationByGroundSurface();

	FVector BoxMin = TargetLocation - 500.f;
	FVector BoxMax = TargetLocation + 500.f;

	RandLocation = FMath::RandPointInBox(FBox(BoxMin, BoxMax));
	RandLocation.Z = TargetLocation.Z;

	SetCollisionLocation(RandLocation);
}

void APattern::SetCollisionLocation(FVector Location)
{
	SphereCollision->SetRelativeLocation(Location);
}

void APattern::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		Target = Cast<AMainPlayer>(OtherActor);
	
		if (Target)
		{
			Monster->MonsterPattern->ApplyPatternDamageToTarget(this);

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
