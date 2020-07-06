// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MainPlayer.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AItem::AItem()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/ItemTable.ItemTable'"));

	if (DataTable.Succeeded())
	{
		ItemTable = DataTable.Object;
	}

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(48.f);
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	IdleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticle"));
	IdleParticle->SetupAttachment(GetRootComponent());

	bIsRotate = false;
	RotateSpeed = 45.f;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemTable)
	{
		ItemTableRow = ItemTable->FindRow<FItemTable>(FName(*(FString::FormatAsNumber(ID))), FString(""));
	}

	// 유니티의 TriggerEnter와 Exit 역할 // 꼭 바인딩 해줘야한다.
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateItem(DeltaTime);
}

void AItem::RotateItem(float DeltaTime)
{
	if (bIsRotate) // 회전해야할 아이템이라면?
	{
		FRotator Rotation = GetActorRotation();

		// Yaw는 제자리 회전 

		Rotation.Yaw += DeltaTime * RotateSpeed;
		SetActorRotation(Rotation);
	}
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* Main = Cast<AMainPlayer>(OtherActor);

		if(Main)
		{
			Main->AddItem(this);
			Main->AddGold(300);

			Destroy();
		}
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

