// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
	EquipMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipMesh"));
	EquipMesh->SetupAttachment(GetRootComponent());


	IdleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticle"));
	IdleParticle->SetupAttachment(GetRootComponent());

	bIsRotate = false;
	RotateSpeed = 45.f;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SetItemData();

	// ����Ƽ�� TriggerEnter�� Exit ���� // �� ���ε� ������Ѵ�.
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateItem(DeltaTime);
}

void AItem::SetItemData()
{
	if (ItemTable)
	{
		FItemTable* ItemTableRow = ItemTable->FindRow<FItemTable>(FName(*(FString::FormatAsNumber(ID))), FString(""));

		if (ItemTableRow)
		{
			ItemTableValue.Name = (*ItemTableRow).Name;

			ItemTableValue.ItemID = (*ItemTableRow).ItemID;
			ItemTableValue.ItemClass = (*ItemTableRow).ItemClass;
			ItemTableValue.ItemType = (*ItemTableRow).ItemType;
			ItemTableValue.Name = (*ItemTableRow).Name;
			ItemTableValue.Description = (*ItemTableRow).Description;
			ItemTableValue.bIsDroppable = (*ItemTableRow).bIsDroppable;
			ItemTableValue.bIsSellable = (*ItemTableRow).bIsSellable;
			ItemTableValue.MaxCount = (*ItemTableRow).MaxCount;
			ItemTableValue.Damage = (*ItemTableRow).Damage;
			ItemTableValue.Deffence = (*ItemTableRow).Deffence;
			ItemTableValue.Strength = (*ItemTableRow).Strength;
			ItemTableValue.Dexterity = (*ItemTableRow).Dexterity;
			ItemTableValue.Intelligence = (*ItemTableRow).Intelligence;

			//Mesh = Cast<UStaticMeshComponent>((*ItemTableRow).Mesh);
			Mesh->SetStaticMesh((*ItemTableRow).Mesh);

			if (ItemTableValue.ItemClass == EItemClass::EIC_Equip && (*ItemTableRow).EquipMesh != nullptr)
			{
				EquipMesh->SetSkeletalMesh((*ItemTableRow).EquipMesh);
			}

			Icon = (*ItemTableRow).Icon;
		}
	}
}

void AItem::RotateItem(float DeltaTime)
{
	if (bIsRotate) // ȸ���ؾ��� �������̶��?
	{
		FRotator Rotation = GetActorRotation();

		// Yaw�� ���ڸ� ȸ�� 

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

			Destroy();
		}
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

