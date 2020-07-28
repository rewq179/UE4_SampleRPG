// Fill out your copyright notice in the Description page of Project Settings.

#include "Npc/NpcController.h"
#include "Manager/ItemManager.h"
#include "Player/MainPlayer.h"
#include "Engine/World.h"

#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ANpcController::ANpcController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	InteractCollision->SetSphereRadius(256.f);
	InteractCollision->SetupAttachment(GetRootComponent());
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/NpcTable.NpcTable'"));

	if (DataTable.Succeeded())
	{
		NpcTable = DataTable.Object;
	}
}

// Called when the game starts or when spawned
void ANpcController::BeginPlay()
{
	Super::BeginPlay();

	if (NpcTable)
	{
		NpcTableRow = NpcTable->FindRow<FNpcTable>(FName(*(FString::FormatAsNumber(NpcID))), FString(""));

		if (NpcTableRow)
		{
			Npc.NpcID = (*NpcTableRow).NpcID;
			Npc.Name = (*NpcTableRow).Name;
			Npc.bHasItem = (*NpcTableRow).bHasItem;
			Npc.bHasQuest = (*NpcTableRow).bHasQuest;
			Npc.DialogueID = (*NpcTableRow).DialogueID;
			Npc.ItemID = (*NpcTableRow).ItemID;

			StringToIntArray(false, Npc.DialogueID);
			StringToIntArray(true, Npc.ItemID);
		}
	}

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ANpcController::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ANpcController::OnOverlapEnd);
}

void ANpcController::StringToIntArray(bool bIsItem, FString Data)
{
	TArray<FString> StringID;

	FString DataString(Data);
	DataString.ParseIntoArray(StringID, TEXT("/"), true);

	for (auto ID : StringID)
	{
		if (bIsItem)
		{
			ItemID.Add(FCString::Atoi(*ID));
		}

		else
		{
			DialogueID.Add(FCString::Atoi(*ID));
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