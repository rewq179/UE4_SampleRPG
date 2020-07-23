// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/LevelManager.h"
#include "Player/MainPlayer.h"

#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Transition = CreateDefaultSubobject<UBoxComponent>(TEXT("Transition"));
	RootComponent = Transition;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	
	Transition->OnComponentBeginOverlap.AddDynamic(this, &ALevelManager::OnOverlapBegin);
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALevelManager::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* Main = Cast<AMainPlayer>(OtherActor);

		if (Main)
		{
			Main->SwitchLevel(LevelName);
		}
	}
}