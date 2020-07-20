// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Monster.h"
#include "MainPlayer.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every f	rame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/ItemTable.ItemTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/ItemTable.ItemTable'"));

	if (DataTable.Succeeded())
	{
		ItemTable = DataTable.Object;
	}

#pragma region Collision/Mesh/Particle
	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	InteractCollision->SetSphereRadius(48.f);
	RootComponent = InteractCollision;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 어떠한 것에도 반응하지 않음
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // Pawn과 Overlap만 반응하도록 한다.
	// Collision

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	EquipMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipMesh"));
	EquipMesh->SetupAttachment(GetRootComponent());

	// Mesh

	IdleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticle"));
	IdleParticle->SetupAttachment(GetRootComponent());
#pragma endregion

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SetItemData();

	// 유니티의 TriggerEnter와 Exit 역할 // 꼭 바인딩 해줘야한다.
	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnCombatOverlapBegin);
	//CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnCombatOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::SetItemData()
{
	if (ItemTable)
	{
		FItemTable* ItemTableRow = ItemTable->FindRow<FItemTable>(FName(*(FString::FormatAsNumber(ItemID))), FString(""));

		if (ItemTableRow)
		{
			ItemTableValue.Name = (*ItemTableRow).Name;

			ItemTableValue.ItemID = (*ItemTableRow).ItemID;
			//ItemID = (*ItemTableRow).ItemID;
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
			ItemTableValue.Mesh = (*ItemTableRow).Mesh;


			//Mesh = Cast<UStaticMeshComponent>((*ItemTableRow).Mesh);
			Mesh->SetStaticMesh((*ItemTableRow).Mesh);

			if (ItemTableValue.ItemClass == EItemClass::EIC_Equip && (*ItemTableRow).EquipMesh != nullptr)
			{
				ItemTableValue.EquipMesh = (*ItemTableRow).EquipMesh;

				EquipMesh->SetSkeletalMesh((*ItemTableRow).EquipMesh);
				EquipMesh->SetVisibility(false);
			}

			Icon = (*ItemTableRow).Icon;
			ItemTableValue.Icon = (*ItemTableRow).Icon;
		}
	}
}

void AItem::SetCombatCollisionEnabled(bool IsActive)
{
	if (IsActive)
	{
		CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	else
	{
		CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		AMainPlayer* Main = Cast<AMainPlayer>(OtherActor);

		if (Main && ItemState == EItemState::EIS_Root)
		{
			Main->RecentItem = this;
		}
	}
}


void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainPlayer* Main = Cast<AMainPlayer>(OtherActor);

		if (Main && ItemState == EItemState::EIS_Root)
		{
			Main->RecentItem = nullptr;
		}
	}
}

void AItem::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && ItemState == EItemState::EIS_Inv && ItemTableValue.ItemType == EItemType::EIT_Weapon)
	{
		AMonster* Monster = Cast<AMonster>(OtherActor);

		if (Monster && ItemOwner )
		{
			ItemOwner->AddTargetMonster(Monster);
		}
	}
}


void AItem::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AItem::IgnoreStaticMesh()
{
	if (ItemTableValue.ItemClass == EItemClass::EIC_Equip)
	{
		Mesh->SetStaticMesh(nullptr);
	}

	EquipMesh->SetVisibility(true);

	EquipMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EquipMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	EquipMesh->SetSimulatePhysics(false); // 물리충돌 X
}
