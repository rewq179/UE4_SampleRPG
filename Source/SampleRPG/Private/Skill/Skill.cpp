// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill.h"

#include "GameManager.h"
#include "MainPlayer.h"
#include "Monster.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASkill::ASkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(GetRootComponent());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());

	UseParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("UseParticle"));
	UseParticle->SetupAttachment(GetRootComponent());

	NotifyFieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("NotifyFieldParticle"));
	NotifyFieldParticle->SetupAttachment(GetRootComponent());

	DamageFieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamageFieldParticle"));
	DamageFieldParticle->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASkill::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkill::OnBoxOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ASkill::OnBoxOverlapEnd);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkill::OnSphereOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ASkill::OnSphereOverlapEnd);
}

void ASkill::SetCollisionSize(FSkillShape SkillShape)
{
	switch (SkillShape.Shape)
	{
	case EShape::ES_Circle:
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereCollision->SetSphereRadius(SkillShape.Size.X);
		break;

	case EShape::ES_Rectangle:
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxCollision->SetBoxExtent(SkillShape.Size);
		break;

	default:
		break;
	}
}


void ASkill::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		MainPlayer = Cast<AMainPlayer>(OtherActor);

		if (MainPlayer)
		{

		}
	}
}


void ASkill::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ASkill::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	
}


void ASkill::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
