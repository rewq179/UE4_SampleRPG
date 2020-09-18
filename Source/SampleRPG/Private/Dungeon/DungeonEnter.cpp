// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonEnter.h"

#include "Manager/GameManager.h"

#include "Player/MainPlayer.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ADungeonEnter::ADungeonEnter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = InteractCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADungeonEnter::BeginPlay()
{
	Super::BeginPlay();

	SetDungeonData();

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ADungeonEnter::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ADungeonEnter::OnOverlapEnd);
}


void ADungeonEnter::SetDungeonData()
{
	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		GameManager = Player->GameManager;
	}

	if (GameManager)
	{
		DungeonData = GameManager->DungeonManager->GetDungeonData(DungeonID);
	}
}

void ADungeonEnter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		MainPlayer = Cast<AMainPlayer>(OtherActor);

		if (MainPlayer)
		{
			MainPlayer->InteractDungeon = this;
		}
	}
}

void ADungeonEnter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == MainPlayer)
	{
		MainPlayer->InteractDungeon = nullptr;
	}
}
