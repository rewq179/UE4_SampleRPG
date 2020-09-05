// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Item.h"
#include "Monster/Monster.h"
#include "Player/MainPlayer.h"

#include "Manager/GameManager.h"
#include "Manager/CombatManager.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

#include "Engine/World.h"
#include "Engine/SkeletalMeshSocket.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "TimerManager.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every f	rame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision //

	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	InteractCollision->SetSphereRadius(48.f);
	RootComponent = InteractCollision;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ��� �Ϳ��� �������� ����
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // Pawn�� Overlap�� �����ϵ��� �Ѵ�.
	
	// Mesh //

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	EquipMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipMesh"));
	EquipMesh->SetupAttachment(GetRootComponent());


	IdleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticle"));
	IdleParticle->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnCombatOverlapBegin);


	SetItemData();
}

void AItem::SetItemData()
{
	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		GameManager = Player->GameManager;
	}

	if (GameManager)
	{
		ItemData = GameManager->ItemManager->GetItemData(ItemID);

		Mesh->SetStaticMesh(ItemData.Mesh);

		if (ItemData.ItemClass == EItemClass::EIC_Equip && ItemData.EquipMesh != nullptr)
		{
			EquipMesh->SetSkeletalMesh(ItemData.EquipMesh);
			EquipMesh->SetVisibility(false);
		}

		Icon = ItemData.Icon;
	}
}

void AItem::SetItemOwner(AMainPlayer* MainPlayer)
{
	ItemOwner = MainPlayer;

	SetItemState(EItemState::EIS_Inv);
	SetActorLocation(FVector(255.f, 255.f, 255.f));
}

void AItem::SetEnabledCombatCollision(bool IsActive)
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


void AItem::IgnoreStaticMesh()
{
	SetEnabledCombatCollision(false);

	Mesh->SetStaticMesh(nullptr);
	
	EquipMesh->SetVisibility(true);

	EquipMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EquipMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	EquipMesh->SetSimulatePhysics(false); // �����浹 X
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && ItemState == EItemState::EIS_Root)
	{
		AMainPlayer* Main = Cast<AMainPlayer>(OtherActor);

		if (Main)
		{
			Main->InteractItems.Add(this);
		}
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && ItemState == EItemState::EIS_Root)
	{
		AMainPlayer* Main =  Cast<AMainPlayer>(OtherActor);

		if (Main)
		{
			Main->InteractItems.Remove(this);
		}
	}
}

void AItem::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && ItemState == EItemState::EIS_Inv && ItemData.ItemType == EItemType::EIT_Weapon)
	{
		auto Monster = Cast<AMonster>(OtherActor);

		if (Monster && ItemOwner)
		{
			GameManager->CombatManager->AddTargetMonster(Monster);
		}
	}
}

