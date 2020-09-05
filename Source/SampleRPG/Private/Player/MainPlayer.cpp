// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MainPlayer.h"
#include "Player/Inventory.h"
#include "Player/PlayerCombat.h"
#include "Player/PlayerStatus.h"
#include "Player/PlayerQuest.h"
#include "Npc/NpcController.h"

#include "Manager/SaveGameManager.h"
#include "Manager/GameManager.h"
#include "Manager/CombatManager.h"

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


//#include "Player/Public/MyActor.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bAbsoluteRotation = true; // 스프링 암의 회전이 루트 컴포넌트와 상위 컴포넌트를 따르지 않고 월드 좌표계의 회전을 따르도록 한다.
	CameraBoom->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	CameraBoom->TargetArmLength = 1000.f; // 셀카봉 길이는 어느정도로?
	CameraBoom->bDoCollisionTest = false; // 카메라가 벽을 뚫지 못하도록 만든다.


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

	LoadGame();
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

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // PlayerInputComponent 유무 체크
	
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AMainPlayer::LeftClickDown);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainPlayer::Roll);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::InteractObject);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
}

void AMainPlayer::MoveForward(float Value)
{
	if (Value != 0.f && Controller && bCanMove && !bIsAttackAnim && !bIsDead && !bIsMenuVisible)
	{
		FRotator Rotation = Controller->GetControlRotation(); // 현재 캐릭터의 회전 매트릭스를 구한다.
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// FRotationMatrix : (0,0,0)과 비교해 각 축들이 얼마나 회전 되어있는지를 계산한다.
		// GetUnitAxis : GetSacledAxis의 결과를 단위 벡터(길이를 1로 변환)하여 돌려주는 함수다.

		AddMovementInput(Direction, Value);

		/*
			다른 방법 :

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

		PlayerStatus->Stat.CurStamina -= RollCost;
		PlayerCombat->PlayMontage(FName("Roll"), 1.f);
	}
}

void AMainPlayer::RollAnimEnd()
{
	bCanMove = true;
	bIsRoll = false;
}

void AMainPlayer::LeftClickDown()
{
	if (bIsEquip && !bIsAttackAnim) // 무기를 소유했으며 공격 애니메이션이 종료되었는가?
	{
		bIsAttackAnim = true;

		PlayerCombat->SetAttackType(EAttackType::EAT_None);
		PlayerCombat->PlayAttackAnim();
	}
}

void AMainPlayer::InteractObject()
{
	if (IsLevelChange(NextLevelName))
	{
		SwitchLevel();

		return;
	}

	if (InteractItems.Num() > 0)
	{
		AddInteractedItemAll();
	}

	else if (InteractNPC)
	{
		GameManager->DialogueManager->SetActiveDialouge(true, InteractNPC);
		
		PlayerQuest->InteractNPC = InteractNPC;
	}

	InteractItems.Empty();
}

void AMainPlayer::AddInteractedItemAll()
{
	for (auto Item : InteractItems)
	{
		Item->SetActiveText(false);

		Inventory->AddItem(Item);
	}
}

void AMainPlayer::StopCommunication()
{
	InteractNPC = nullptr;

	GameManager->DialogueManager->SetActiveDialouge(false, nullptr);
}

void AMainPlayer::SwitchLevel()
{
	UWorld* World = GetWorld();

	if (World)
	{
		UGameplayStatics::OpenLevel(World, NextLevelName);

		NextLevelName = FName("None");
	}
}

bool AMainPlayer::IsLevelChange(FName NextLevelName)
{
	if (NextLevelName.IsEqual(FName("None")))
	{
		return false;
	}

	UWorld* World = GetWorld();

	if (World)
	{
		FString CurLevelName(*World->GetMapName());
		CurLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if (FName(*CurLevelName) != NextLevelName)
		{
			return true;
		}
	}

	return false;
}

void AMainPlayer::SaveGame()
{
	USaveGameManager* SaveGameInstance =  Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));

	PlayerStatus->SavePlayerStatData(SaveGameInstance);

	SaveGameInstance->Location = GetActorLocation();
	SaveGameInstance->Rotation = GetActorRotation();

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // 맵 이름앞에 XXXXX_X_이름 이런식으로 있기 때문

	SaveGameInstance->LevelName = MapName;

	Inventory->SaveInventoryData(SaveGameInstance);
	PlayerQuest->SaveQuestData(SaveGameInstance);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void AMainPlayer::LoadGame()
{
	USaveGameManager* LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));
	LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	PlayerStatus->LoadPlayerStatData(LoadGameInstance);

	NextLevelName = FName(*LoadGameInstance->LevelName);
	if (IsLevelChange(NextLevelName))
	{
		SwitchLevel();

		SetActorLocation(LoadGameInstance->Location);
		SetActorRotation(LoadGameInstance->Rotation);
	}
	
	Inventory->LoadInventoryData(LoadGameInstance);
	PlayerQuest->LoadQuestData(LoadGameInstance);
}
