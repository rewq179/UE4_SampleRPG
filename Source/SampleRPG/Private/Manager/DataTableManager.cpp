// Fill out your copyright notice in the Description page of Project Settings.

#include "DataTableManager.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ADataTableManager::ADataTableManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/NpcTable.NpcTable'"));

	if (DataTable.Succeeded())
	{
		NpcTable = DataTable.Object;
	}
}

// Called when the game starts or when spawned
void ADataTableManager::BeginPlay()
{
	Super::BeginPlay();
}

FNpcTable* ADataTableManager::GetNpcData(int32 NpcID)
{
	NpcData = NpcTable->FindRow<FNpcTable>(FName(*(FString::FormatAsNumber(NpcID))), FString(""));

	return NpcData;
}

// Called every frame
void ADataTableManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

