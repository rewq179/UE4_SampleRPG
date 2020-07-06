// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

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

#pragma region InputKeyBool
	bIsSprint = false;

#pragma endregion

#pragma region Status
	NormalSpeed = 600.f;
	SprintSpeed = 1000.f;
#pragma endregion

	// #include "UObject/ConstructorHelpers.h"

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/PlayerStatusTable.PlayerStatusTable'"));

	if (DataTable.Succeeded())
	{
		PlayerStatusTable = DataTable.Object;
	}
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStatus(1);
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

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::LeftClickDown);
	PlayerInputComponent->BindAction("InventoryUI", IE_Pressed, this, &AMainPlayer::ToggleInventoryUI);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("RightLeftView", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("UpDownView", this, &APawn::AddControllerPitchInput);
}

#pragma region Sprint
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


#pragma endregion

#pragma region Status
void AMainPlayer::TakeDamage(float Damage)
{
	int total = Damage - Deffence;

	if (total > 0)
	{
		CurHP -= total;

		if (CurHP <= 0)
		{
			Death();
		}
	}
}

void AMainPlayer::Death()
{
	CurHP = MaxHP;

	UE_LOG(LogTemp, Warning, TEXT("Player Die!"));

}

void AMainPlayer::LevelUP()
{
	
}

void AMainPlayer::SetLevelStatus(int CurLevel)
{
	if (PlayerStatusTable)
	{
		UE_LOG(LogTemp, Log, TEXT("AAAAAAAAA"));

		PlayerStatusTableRow = PlayerStatusTable->FindRow<FPlayerStatusTable>(FName(*(FString::FormatAsNumber(CurLevel - 1))), FString(""));
	}

	if (PlayerStatusTableRow)
	{
		UE_LOG(LogTemp, Log, TEXT("HIO"));
	}

	Level = (*PlayerStatusTableRow).Level;
	MaxExp = (*PlayerStatusTableRow).Exp;
	CurExp = 0;
	MaxHP = (*PlayerStatusTableRow).MaxHP;
	CurHP = (*PlayerStatusTableRow).CurHP;
	MaxStamina = (*PlayerStatusTableRow).MaxStamina;
	CurStamina = (*PlayerStatusTableRow).CurStamina;
	Damage = (*PlayerStatusTableRow).Damage;
	Deffence = (*PlayerStatusTableRow).Deffence;
	Strength = (*PlayerStatusTableRow).Strength;
	Dexterity = (*PlayerStatusTableRow).Dexterity;
	Intelligence = (*PlayerStatusTableRow).Intelligence;
}

#pragma endregion


#pragma region UI
void AMainPlayer::ToggleInventoryUI()
{
	
}

#pragma endregion


void AMainPlayer::MoveForward(float Value)
{
	if (Value != 0.f && Controller)
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
	if (Value != 0.f && Controller)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::LeftClickDown()
{

}


void AMainPlayer::AddItem(AItem* Item)
{
	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		if (Inventory[Index]->ID == Item->ID)
		{
			Inventory[Index]->Count += Item->Count;
		}
	}

	Inventory.Add(Item);
}

void AMainPlayer::RemoveItem(AItem* Item)
{

}

void AMainPlayer::EquipItem(AItem* Item)
{

}

void AMainPlayer::UnEquipItem(AItem* Item)
{

}