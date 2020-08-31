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

#pragma region Camera
	/*
		���� : ��ī���� ��� �ڸ� ���������, ī�޶� �������Ͽ� �������ѳ������� ���������� �ű��.
	*/

	// ������� : "GameFramework/SpringArmComponent.h"
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bAbsoluteRotation = true; // ������ ���� ȸ���� ��Ʈ ������Ʈ�� ���� ������Ʈ�� ������ �ʰ� ���� ��ǥ���� ȸ���� �������� �Ѵ�.
	CameraBoom->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	CameraBoom->TargetArmLength = 1000.f; // ��ī�� ���̴� ���������?
	CameraBoom->bDoCollisionTest = false; // ī�޶� ���� ���� ���ϵ��� �����.

	//CameraBoom->bUsePawnControlRotation = true; // ī�޶���� �����̼��� �����

	// ������� : #include "Camera/CameraComponent.h"
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// ī�޶󿡵��� ĳ���͸� �������� �ʰڴ�.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// ������� : #include "GameFramework/CharacterMovementComponent.h"
	GetCharacterMovement()->bOrientRotationToMovement = true; // ĳ������ �̵������ �þ߹����� �ٸ��� �ʴ� 640���� ȸ�� �ӵ��� ȸ����Ų��.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;
#pragma endregion

	bCanMove = true;
	bIsSprint = false;
	RollCost = 50.f;
	NormalSpeed = 600.f;
	SprintSpeed = 1000.f;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryBP)
	{
		Inventory = GetWorld()->SpawnActor<AInventory>(InventoryBP);

		if (Inventory)
		{
			Inventory->MainPlayer = this;
			Inventory->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			
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
		}
	}

	if (PlayerQuestBP)
	{
		PlayerQuest = GetWorld()->SpawnActor<APlayerQuest>(PlayerQuestBP);

		if (PlayerCombat)
		{
			PlayerQuest->MainPlayer = this;
			PlayerQuest->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			if (GameManager && GameManager->QuestManager)
			{
				PlayerQuest->QuestManager = GameManager->QuestManager;
			}
		}
	}

	PlayerCombat->CombatManager = GameManager->CombatManager;

	LoadGame();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // PlayerInputComponent ���� üũ

	// ������� : #include "GameFramework/PlayerController.h"

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AMainPlayer::LeftClickDown);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainPlayer::Roll);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::InteractObject);

	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	// PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("RightLeftView", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("UpDownView", this, &APawn::AddControllerPitchInput);
}

#pragma region Movement/Roll Function
void AMainPlayer::MoveForward(float Value)
{
	if (Value != 0.f && Controller && bCanMove && !bIsAttackAnim && MovementState != EMovementState::EMS_Dead && !bIsMenuVisible)
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
	if (Value != 0.f && Controller && bCanMove && !bIsAttackAnim && MovementState != EMovementState::EMS_Dead && !bIsMenuVisible)
	{

		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}


void AMainPlayer::LeftShiftKeyDown()
{
	SetMovementState(EMovementState::EMS_Sprint);

	bIsSprint = true;
}

void AMainPlayer::LeftShiftKeyUp()
{
	SetMovementState(EMovementState::EMS_Idle);

	bIsSprint = false;
}

void AMainPlayer::Roll()
{
	if (!bIsRoll && bCanMove  && MovementState != EMovementState::EMS_Dead && PlayerStatus->Stat.CurStamina >= RollCost && !bIsMenuVisible)
	{
		PlayerStatus->Stat.CurStamina -= RollCost;
		bIsRoll = true;
		//bCanMove = false;

		PlayerCombat->PlayMontage(FName("Roll"), 1.f);
	}
}

void AMainPlayer::RollAnimEnd()
{
	bCanMove = true;
	bIsRoll = false;
}
#pragma endregion

#pragma region Interact/Attack Mouse Button 
void AMainPlayer::LeftClickDown()
{
	bIsLeftClickDown = true;

	if (bIsEquip && !bIsAttackAnim) // ���⸦ ���������� ���� �ִϸ��̼��� ����Ǿ��°�?
	{
		bIsAttackAnim = true;

		PlayerCombat->SetAttackType(EAttackType::EAT_Normal);
		PlayerCombat->PlayAttackAnim();
	}
}

void AMainPlayer::LeftClickUp()
{
	bIsLeftClickDown = false;
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
		AddItem();
	}
	

	else if (InteractNPC)
	{
		GameManager->DialogueManager->SetActiveDialouge(true, InteractNPC);
		
		PlayerQuest->InteractNPC = InteractNPC;
	}

	InteractItems.Empty();
}

void AMainPlayer::AddItem()
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
#pragma endregion

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
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // �� �̸��տ� XXXXX_X_�̸� �̷������� �ֱ� ����

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

void AMainPlayer::LoadGameNoSwitch()
{
	USaveGameManager* LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));
	LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	PlayerStatus->LoadPlayerStatData(LoadGameInstance);
}

void AMainPlayer::SetMovementState(EMovementState State) // ĳ���ͻ��¿����� �ִ�ӵ��� ��ȭ��
{
	MovementState = State;

	// ������� : #include "GameFramework/PlayerController.h"

	if (MovementState == EMovementState::EMS_Sprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // �ִ� �ӵ��� ������Ʈ �ӵ���
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // �ִ� �ӵ��� �Ϲ� �޸��� �ӵ���
	}
}