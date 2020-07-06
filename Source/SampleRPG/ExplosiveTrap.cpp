// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveTrap.h"
#include "MainPlayer.h"

// Sets default values
AExplosiveTrap::AExplosiveTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 10.f;
}

void AExplosiveTrap::BeginPlay()
{
	Super::BeginPlay();

}


void AExplosiveTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);

		if (Player)
		{
			Player->TakeDamage(Damage);
		}
	}
}

void AExplosiveTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}