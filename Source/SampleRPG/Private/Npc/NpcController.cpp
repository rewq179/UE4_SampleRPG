// Fill out your copyright notice in the Description page of Project Settings.

#include "Npc/NpcController.h"
#include "Player/MainPlayer.h"
#include "Engine/World.h"

#include "Manager/GameManager.h"

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

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ANpcController::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ANpcController::OnOverlapEnd);
}

void ANpcController::SetNpcData()
{
	if (!bIsDataSetting)
	{
		bIsDataSetting = true;

		FNpcTable* NpcTableRow = GameManager->DataTableManager->GetNpcData(NpcID);

		if (NpcTableRow)
		{
			UE_LOG(LogTemp, Log, TEXT("%d"), (*NpcTableRow).NpcID);

			NpcData.NpcID = (*NpcTableRow).NpcID;
			NpcData.Name = (*NpcTableRow).Name;
			NpcData.bHasItem = (*NpcTableRow).bHasItem;
			NpcData.bHasQuest = (*NpcTableRow).bHasQuest;
			NpcData.DialogueID = (*NpcTableRow).DialogueID;
			NpcData.ItemID = (*NpcTableRow).ItemID;
			NpcData.QuestID = (*NpcTableRow).QuestID;

			StringToIntArray(0, NpcData.DialogueID);
			StringToIntArray(1, NpcData.ItemID);
			StringToIntArray(2, NpcData.QuestID);

		}
	}
}

void ANpcController::StringToIntArray(int32 Type, FString Data)
{
	TArray<FString> StringID;

	FString DataString(Data);
	DataString.ParseIntoArray(StringID, TEXT("/"), true);

	for (auto ID : StringID)
	{
		if (Type == 0) // Item
		{
			DialogueID.Add(FCString::Atoi(*ID));
		}

		else if(Type == 1) // Dialouge
		{
			ItemID.Add(FCString::Atoi(*ID));
		}

		else
		{
			QuestID.Add(FCString::Atoi(*ID));
		}
	}
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

			if (!bIsDataSetting)
			{
				GameManager = MainPlayer->GameManager;
				SetNpcData();
			}
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