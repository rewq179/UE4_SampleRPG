// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"

#include "Player/MainPlayer.h"
#include "Manager/GameManager.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		GameManager = Player->GameManager;
	}

	GameManager->DungeonManager->AddSpawnPoint(PointID, this);
}
