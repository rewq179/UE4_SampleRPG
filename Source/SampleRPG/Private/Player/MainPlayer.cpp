// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MainPlayer.h"
#include "Player/Inventory.h"
#include "Player/PlayerCombat.h"
#include "Player/PlayerStatus.h"
#include "Player/PlayerQuest.h"
#include "Npc/NpcController.h"

#include "Manager/RGameInstance.h"
#include "Manager/SaveGameManager.h"
#include "Manager/GameManager.h"
#include "Manager/CombatManager.h"

#include "Dungeon/DungeonEnter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"

#include "TimerManager.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bAbsoluteRotation = true; // ������ ���� ȸ���� ��Ʈ ������Ʈ�� ���� ������Ʈ�� ������ �ʰ� ���� ��ǥ���� ȸ���� �������� �Ѵ�.
	CameraBoom->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	CameraBoom->TargetArmLength = 1000.f; // ��ī�� ���̴� ���������?
	CameraBoom->bDoCollisionTest = false; // ī�޶� ���� ���� ���ϵ��� �����.


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	bCanMove = true;
	RollCost = 50.f;
	NormalSpeed = 600.f;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitComponents();
	InitData();

	Inventory->AddGold(10000);
}

void AMainPlayer::InitComponents()
{
	if (InventoryBP)
	{
		Inventory = GetWorld()->SpawnActor<AInventory>(InventoryBP);

		if (Inventory)
		{
			Inventory->MainPlayer = this;
			Inventory->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

			if (GameManager && GameManager->ItemManager)
			{
				Inventory->ItemManager = GameManager->ItemManager;
			}
		}
	}

	if (PlayerCombatBP)
	{
		PlayerCombat = GetWorld()->SpawnActor<APlayerCombat>(PlayerCombatBP);

		if (PlayerCombat)
		{
			PlayerCombat->MainPlayer = this;
			PlayerCombat->Inventory = Inventory;
			PlayerCombat->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			PlayerCombat->CombatManager = GameManager->CombatManager;
		}
	}

	if (PlayerStatusBP)
	{
		PlayerStatus = GetWorld()->SpawnActor<APlayerStatus>(PlayerStatusBP);

		if (PlayerStatus)
		{
			PlayerStatus->MainPlayer = this;
			PlayerStatus->SkillManager = GameManager->SkillManager;
			PlayerStatus->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			PlayerStatus->SetLevelStatus(1);

			PlayerCombat->PlayerStatus = PlayerStatus;
		}
	}

	if (PlayerQuestBP)
	{
		PlayerQuest = GetWorld()->SpawnActor<APlayerQuest>(PlayerQuestBP);

		if (PlayerQuest)
		{
			PlayerQuest->MainPlayer = this;
			PlayerQuest->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			if (GameManager && GameManager->QuestManager)
			{
				PlayerQuest->QuestManager = GameManager->QuestManager;
			}
		}
	}
}

void AMainPlayer::InitData()
{
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (!MapName.Equals("Lobby"))
	{
		auto GameInstance = Cast<URGameInstance>(GetGameInstance());

		CurSlotName = GameInstance->SlotName;

		LoadGame();
	}
}
// Input Key //

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // PlayerInputComponent ���� üũ
	
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AMainPlayer::LeftClickDown);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainPlayer::Roll);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::InteractObject);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
}

void AMainPlayer::LeftClickDown()
{
	if (bIsEquip && !bIsAttackAnim && !bIsRoll) // ���⸦ ���������� ���� �ִϸ��̼��� ����Ǿ��°�?
	{
		bIsAttackAnim = true;

		PlayerCombat->SetAttackType(EAttackType::EAT_None);

		if (bCanRollAttack)
		{
			bCanRollAttack = false;

			PlayerCombat->PlayAttackAnim(0, 2.f);
		}

		else
		{
			if (PlayerCombat->bCanComboAttack)
			{
				PlayerCombat->PlayAttackAnim(PlayerCombat->ComboCount, 1.25f);
			}

			else
			{
				PlayerCombat->PlayAttackAnim(1, 1.25f);
			}
		}
	}
}

void AMainPlayer::MoveForward(float Value)
{
	if (Value != 0.f && Controller && bCanMove && !bIsAttackAnim && !bIsDead && !bIsMenuVisible)
	{
		FRotator Rotation = Controller->GetControlRotation(); // ���� ĳ������ ȸ�� ��Ʈ������ ���Ѵ�.
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// FRotationMatrix : (0,0,0)�� ���� �� ����� �󸶳� ȸ�� �Ǿ��ִ����� ����Ѵ�.
		// GetUnitAxis : GetSacledAxis�� ����� ���� ����(���̸� 1�� ��ȯ)�Ͽ� �����ִ� �Լ���.

		AddMovementInput(Direction, Value);

		/*
			�ٸ� ��� :

			FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		*/
	}
}

void AMainPlayer::MoveRight(float Value)
{
	if (Value != 0.f && Controller && bCanMove && !bIsAttackAnim && !bIsDead && !bIsMenuVisible)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::Roll()
{
	if (!bIsRoll && !bIsDead && PlayerStatus->Stat.CurStamina >= RollCost && !bIsMenuVisible)
	{
		bIsRoll = true;
		bCanMove = false;
		bIsAttackAnim = false;

		PlayerStatus->Stat.CurStamina -= RollCost;
		PlayerCombat->PlayMontage(FName("Roll"), 1.5f);
	}
}

void AMainPlayer::RollAnimEnd()
{
	bCanMove = true;
	bCanRollAttack = true;
	bIsRoll = false;

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainPlayer::SetRollAttack, 1.f);
}

// Interact //

void AMainPlayer::InteractObject()
{
	if (InteractDungeon)
	{
		GameManager->DungeonManager->SetActiveDungeonHUD(InteractDungeon->DungeonData);
	}

	else if (InteractItems.Num() > 0)
	{
		AddInteractedItemAll();

		InteractItems.Empty();
	}

	else if (InteractNPC)
	{
		GameManager->DialogueManager->SetNPC(InteractNPC);
		
		PlayerQuest->InteractNPC = InteractNPC;
	}
}

void AMainPlayer::AddInteractedItemAll()
{
	for (auto Item : InteractItems)
	{
		Item->SetActiveText(false);

		Inventory->AddItem(Item);
	}
}

// Save & Load GameData //

void AMainPlayer::SaveGame(FString SlotName)
{
	USaveGameManager* SaveGameInstance =  Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));

	PlayerStatus->SavePlayerStatData(SaveGameInstance);

	SaveGameInstance->Location = GetActorLocation();
	SaveGameInstance->Rotation = GetActorRotation();

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // �� �̸��տ� XXXXX_X_�̸� �̷������� �ֱ� ����

	SaveGameInstance->LevelName = MapName;

	Inventory->SaveInventoryData(SaveGameInstance);
	PlayerQuest->SaveQuestData(SaveGameInstance);

	if (SlotName.Equals(""))
	{
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, CurSlotName, 0);
	}

	else
	{
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
	}
}


void AMainPlayer::LoadGame()
{
	USaveGameManager* LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));
	LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(CurSlotName, 0));

	PlayerStatus->LoadPlayerStatData(LoadGameInstance);
	Inventory->LoadInventoryData(LoadGameInstance);
	PlayerQuest->LoadQuestData(LoadGameInstance);
}

FPlayerStatTable AMainPlayer::LoadData(FString SlotName)
{
	USaveGameManager* LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));
	LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	return LoadGameInstance->PlayerStat;
}
