// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerQuest.h"
#include "Engine/World.h"

// Sets default values
APlayerQuest::APlayerQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerQuest::BeginPlay()
{
	Super::BeginPlay();

	if (QuestManagerBP)
	{
		QuestManager = GetWorld()->SpawnActor<AQuestManager>(QuestManagerBP);
	}
}

// Called every frame
void APlayerQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerQuest::AddQuest(FQuestTable Quest)
{
	RecentQuest = Quest;

	//Quests.AddUnique(Quest);
}
