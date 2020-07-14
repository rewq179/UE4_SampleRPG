// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatManager.h"

// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatManager::TakeDamageToTarget(AActor* Attacker, AActor* Deffencer, float Damage, bool isTargetPlayer)
{
	if (isTargetPlayer)
	{
		AMainPlayer* Target = Cast<AMainPlayer>(Deffencer);

		if (Target)
		{
			UE_LOG(LogTemp, Log, TEXT("Take Damage(%d) to Player"), Damage)

			Target->TakeDamage(Damage);
		}
	}

	else
	{
		AMonster* Target = Cast<AMonster>(Deffencer);

		if (Target)
		{
			UE_LOG(LogTemp, Log, TEXT("Take Damage(%d) to Monster"), Damage)

			Target->TakeDamage(Damage);
		}
	}
}

