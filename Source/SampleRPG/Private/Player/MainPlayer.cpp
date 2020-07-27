// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MainPlayer.h"
#include "Player/Inventory.h"
#include "Player/PlayerCombat.h"
#include "Player/PlayerStatus.h"

#include "Manager/SaveGameManager.h"
#include "Manager/ItemManager.h"
#include "Manager/DialogueManager.h"

#include "Components/SkeletalMeshComponent.h"
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

		if (PlayerCombat)
		{
			PlayerStatus->MainPlayer = this;
			PlayerStatus->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (DialogueManagerBP)
	{
		DialogueManager = GetWorld()->SpawnActor<ADialogueManager>(DialogueManagerBP);

		if (PlayerCombat)
		{
			DialogueManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // �� �̸��տ� XXXXX_X_�̸� �̷������� �ֱ� ����

	//LoadGameNoSwitch();
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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::StartCommunication);

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
	if (Value != 0.f && Controller && !bIsAttackAnim && MovementState != EMovementState::EMS_Dead && !bIsMenuVisible)
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
	if (Value != 0.f && Controller && !bIsAttackAnim && MovementState != EMovementState::EMS_Dead && !bIsMenuVisible)
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
	if (!bIsRoll && MovementState != EMovementState::EMS_Dead && PlayerStatus->Stat.CurStamina >= RollCost && !bIsMenuVisible)
	{
		PlayerStatus->Stat.CurStamina -= RollCost;
		bIsRoll = true;

		PlayerCombat->PlayMontage(FName("Roll"), 1.f);
	}
}

void AMainPlayer::RollEnd()
{
	bIsRoll = false;
}
#pragma endregion

#pragma region Interact/Attack Mouse Button 
void AMainPlayer::LeftClickDown()
{
	bIsLeftClickDown = true;

	if (InteractItem)
	{
		Inventory->AddItem(InteractItem);

		InteractItem = nullptr;
	}

	else if (bIsEquip && !bIsAttackAnim) // ���⸦ ���������� ���� �ִϸ��̼��� ����Ǿ��°�?
	{
		PlayerCombat->PlayAttackAnim();
	}
}

void AMainPlayer::LeftClickUp()
{
	bIsLeftClickDown = false;
}

void AMainPlayer::StartCommunication()
{
	if (InteractNPC && DialogueManager)
	{
		DialogueManager->SetActiveDialouge(this, InteractNPC);
		
	}
}

void AMainPlayer::StopCommunication()
{
	InteractNPC = nullptr;

	DialogueManager->SetActiveDialouge(nullptr, nullptr);
}
#pragma endregion

void AMainPlayer::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FName CurLevelName(*World->GetMapName());

		if (CurLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
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

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void AMainPlayer::LoadGame(bool SetPosition)
{
	USaveGameManager* LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::CreateSaveGameObject(USaveGameManager::StaticClass()));
	LoadGameInstance = Cast<USaveGameManager>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
	

	PlayerStatus->LoadPlayerStatData(LoadGameInstance);

	if (SetPosition)
	{
		SetActorLocation(LoadGameInstance->Location);
		SetActorRotation(LoadGameInstance->Rotation);
	}

	if (!LoadGameInstance->LevelName.Equals("ElvenRuins"))
	{
		FName LevelName(*LoadGameInstance->LevelName);
		SwitchLevel(LevelName);
	}

	Inventory->LoadInventoryData(LoadGameInstance);
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

void AMainPlayer::AddExp(int32 Exp)
{
	PlayerStatus->AddExp(Exp);
}

void AMainPlayer::AdjustHP(float Amount, bool CanDie)
{
	PlayerStatus->AdjustHP(Amount, CanDie);
}

void AMainPlayer::AddTargetMonster(class AMonster* Monster)
{
	PlayerCombat->AddTargetMonster(Monster);
}
void AMainPlayer::AddWidgetMonster(class AMonster* Monster)
{
	PlayerCombat->AddWidgetMonster(Monster);
}
void AMainPlayer::RemoveWidgetMonster(class AMonster* Monster)
{
	PlayerCombat->RemoveWidgetMonster(Monster);
}