// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockingTrigger.h"

#include "Player/MainPlayer.h"
#include "Manager/GameManager.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABlockingTrigger::ABlockingTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
}

// Called when the game starts or when spawned
void ABlockingTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		GameManager = Player->GameManager;
	}

	GameManager->DungeonManager->AddBlockingTrigger(BlockID, this);
}

void ABlockingTrigger::SetCollisionOverlap()
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}