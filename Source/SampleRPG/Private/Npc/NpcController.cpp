// Fill out your copyright notice in the Description page of Project Settings.

#include "Npc/NpcController.h"
#include "Player/MainPlayer.h"
#include "Engine/World.h"

#include "Manager/GameManager.h"
#include "Manager/NpcManager.h"
#include "Manager/QuestManager.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANpcController::ANpcController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	InteractCollision->SetSphereRadius(256.f);
	InteractCollision->SetupAttachment(GetRootComponent());

	QuestionSymbol = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Question"));
	QuestionSymbol->SetupAttachment(GetRootComponent());

	ExclamationSymbol = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Exclamation"));
	ExclamationSymbol->SetupAttachment(GetRootComponent());

	ExclamationSymbol->SetVisibility(false);
}

// Called when the game starts or when spawned
void ANpcController::BeginPlay()
{
	Super::BeginPlay();

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ANpcController::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ANpcController::OnOverlapEnd);
	
	SetActiveSymbol(ESymbolType::EQT_None);
	SetNpcData();
}

#pragma region Data Setting

void ANpcController::SetNpcData()
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (MainPlayer)
	{
		GameManager = MainPlayer->GameManager;
	}

	if (GameManager)
	{
		NpcData = GameManager->NpcManager->GetNpcData(NpcID);

		StringToIntArray(0, NpcData.DialogueID);
		StringToIntArray(1, NpcData.ItemID);
		StringToIntArray(2, NpcData.QuestID);

		GameManager->NpcManager->NpcMap.Add(NpcData.NpcID, this);
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

void ANpcController::SetActiveSymbol(ESymbolType SymbolType)
{
	switch (SymbolType)
	{
	case ESymbolType::EQT_None:
		QuestionSymbol->SetVisibility(false);
		ExclamationSymbol->SetVisibility(false);
		break;

	case ESymbolType::EQT_Question:
		QuestionSymbol->SetVisibility(true);
		ExclamationSymbol->SetVisibility(false);
		break;

	case ESymbolType::EQT_Exclamation:
		QuestionSymbol->SetVisibility(false);
		ExclamationSymbol->SetVisibility(true);
		break;

	default:
		break;
	}
}

#pragma endregion

#pragma region Overlap

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

#pragma endregion

