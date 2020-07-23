// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/DialogueManager.h"


// Sets default values
ADialogueManager::ADialogueManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADialogueManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADialogueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogueManager::SetActiveDialouge(class AMainPlayer* Player, class ANpcController* Npc)
{
	if (this->Npc != Npc)
	{
		this->Npc = Npc;
	}

	if (MainPlayer != Player)
	{
		MainPlayer = Player;
	}

	
}
