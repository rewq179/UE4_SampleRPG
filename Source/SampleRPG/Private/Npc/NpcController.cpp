// Fill out your copyright notice in the Description page of Project Settings.

#include "Npc/NpcController.h"
#include "Manager/ItemManager.h"
#include "Player/MainPlayer.h"
#include "Engine/World.h"

#include "Components/SphereComponent.h"

// Sets default values
ANpcController::ANpcController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	InteractCollision->SetSphereRadius(256.f);
	InteractCollision->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ANpcController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemManagerBP)
	{
		ItemManager = GetWorld()->SpawnActor<AItemManager>(ItemManagerBP);
	}

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ANpcController::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ANpcController::OnOverlapEnd);
}

// Called every frame
void ANpcController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANpcController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANpcController::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		MainPlayer = Cast<AMainPlayer>(OtherActor);

		if (MainPlayer)
		{
			MainPlayer->InteractNPC = this;
		}
	}
}


void ANpcController::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		MainPlayer = Cast<AMainPlayer>(OtherActor);

		if (MainPlayer)
		{
			
			MainPlayer->StopCommunication();

			MainPlayer = nullptr;
		}
	}
}