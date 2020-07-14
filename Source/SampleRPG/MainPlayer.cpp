// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"

#include "Animation/AnimInstance.h"

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

	Equipments.Init(nullptr, 10); 
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

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AMainPlayer::LeftClickDown);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainPlayer::Roll);

	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	// PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("RightLeftView", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("UpDownView", this, &APawn::AddControllerPitchInput);
}

#pragma region Movement/Roll
void AMainPlayer::MoveForward(float Value)
{
	if (Value != 0.f && Controller && !bIsAttackAnim)
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
	if (Value != 0.f && Controller && !bIsAttackAnim)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
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
	if (!bIsRoll)
	{
		bIsRoll = true;

		PlayMontage(FName("Roll"), 1.f);
	}
}

void AMainPlayer::RollEnd()
{
	bIsRoll = false;
}
#pragma endregion

#pragma region Status
void AMainPlayer::AddExp(int32 Exp)
{
	Status.CurExp += Exp;

	CheckLevelUP();
}

void AMainPlayer::CheckLevelUP()
{
	while (Status.CurExp >= Status.MaxExp)
	{
		int32 Exp = Status.CurExp - Status.MaxExp;

		SetLevelStatus(Status.Level + 1);
		
		Status.CurExp = Exp;
	}
}

void AMainPlayer::SetLevelStatus(int32 CurLevel)
{
	if (PlayerStatusTable)
	{
		PlayerStatusTableRow = PlayerStatusTable->FindRow<FPlayerStatusTable>(FName(*(FString::FormatAsNumber(CurLevel - 1))), FString(""));
	}
	
	Status.Level = (*PlayerStatusTableRow).Level;
	Status.MaxExp = (*PlayerStatusTableRow).MaxExp;
	Status.CurExp = (*PlayerStatusTableRow).CurExp;
	Status.MaxHP = (*PlayerStatusTableRow).MaxHP;
	Status.CurHP = (*PlayerStatusTableRow).CurHP;
	Status.MaxStamina = (*PlayerStatusTableRow).MaxStamina;
	Status.CurStamina = (*PlayerStatusTableRow).CurStamina;
	Status.Damage = (*PlayerStatusTableRow).Damage;
	Status.Deffence = (*PlayerStatusTableRow).Deffence;
	Status.Strength = (*PlayerStatusTableRow).Strength;
	Status.Dexterity = (*PlayerStatusTableRow).Dexterity;
	Status.Intelligence = (*PlayerStatusTableRow).Intelligence;
}

void AMainPlayer::TakeDamage(float Damage)
{
	int total = Damage - Status.Deffence;

	if (total > 0)
	{
		Status.CurHP -= total;

		if (Status.CurHP <= 0)
		{
			Death();
		}
	}
}

void AMainPlayer::Death()
{
	Status.CurHP = Status.MaxHP;

	UE_LOG(LogTemp, Warning, TEXT("Player Die!"));

}

#pragma endregion


#pragma region Interact/Attack Mouse Button 
void AMainPlayer::LeftClickDown()
{
	bIsLeftClickDown = true;

	if (RecentItem)
	{
		RecentItem->SetItemState(EItemState::EIS_Inv);

		if (RecentItem->ItemTableValue.ItemType == EItemType::EIT_Weapon)
		{
			RecentItem->IgnoreStaticMesh();
			RecentItem->ItemOwner = this;
			RecentItem->SetCombatCollisionEnabled(false);
			EquipItem(RecentItem);
		}

		else
		{
			AddItem(RecentItem);
		}

		RecentItem = nullptr;
	}

	else if (Equipments[0])
	{
		if (!bIsAttackAnim)
		{
			PlayAttackAnim();
		}
	}
}

void AMainPlayer::LeftClickUp()
{
	bIsLeftClickDown = false;
}
#pragma endregion

#pragma region AttackFunction

void AMainPlayer::PlayAttackAnim()
{
	// ������� : #include "Engine/SkeletalMeshSocket.h"

	PlayMontage(FName("Attack_1"), 2.f);
}

void AMainPlayer::AttackDamage()
{
	for (auto Monster : TargetMonsters)
	{
		Monster->TakeDamage(GetTotalDamage());
	}

	if (Equipments[0]->AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, Equipments[0]->AttackSound);
	}
}

void AMainPlayer::AttackStart()
{
	bIsAttackAnim = true;

	Equipments[0]->SetCombatCollisionEnabled(true);
}

void AMainPlayer::AttackEnd()
{
	bIsAttackAnim = false;

	Equipments[0]->SetCombatCollisionEnabled(false);
	TargetMonsters.Empty();

}
#pragma endregion


#pragma region Inventory Function
void AMainPlayer::AddItem(AItem* Item) 
{
	/*
		���� :  �κ��丮�� �Ⱦ �������� �����ϴ��� Ȯ���Ѵ�.

		if) O : Item�� Count�� �����Ѵ�. Count�� Max�� �Ѿ�� ���ο� ������ Item�� ��ġ�Ѵ�.
		if) X : ���ο� ������ �ش� Item�� ��ġ�Ѵ�.
	*/

	for (int32 Index = 0; Index != Inventory.Num(); Index++)
	{
		if (Inventory[Index]->ItemID == Item->ItemID)
		{
			int32 TotalCount = Inventory[Index]->Count + Item->Count;
			int32 MaxCount = Item->ItemTableValue.MaxCount;

			if (TotalCount <= MaxCount) 
			{
				Inventory[Index]->Count += Item->Count;
			}

			else
			{ 
				Inventory[Index]->Count = MaxCount;

				Item->Count = TotalCount - MaxCount;
				Item->ItemOwner = this;
				Inventory.Add(Item);

				//UE_LOG(LogTemp, Log, TEXT("Apple Count : %d // A2 Count : %d"), Inventory[0]->Count, Inventory[1]->Count);
			}

			return;
		}
	}
	
	Inventory.Add(Item);
}

void AMainPlayer::RemoveItem(AItem* Item)
{

}

void AMainPlayer::EquipItem(AItem* NewItem)
{
	/*
		0:����, 1:����, 2:���, 3:����, 4:�����, 5:�尩, 6:����, 7:�Ź�, 8:�Ǽ� 1, 9:�Ǽ� 2
	*/

	int slotIndex = GetEquipmentIndex(NewItem);

	// ������� : #include "Engine/SkeletalMeshSocket.h"

	const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");

	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(NewItem, GetMesh());

		if (Equipments[slotIndex])
		{
			AItem* OldItem = Equipments[slotIndex];
			WasStatusChangedByEquip(OldItem, false);
		}

		Equipments[slotIndex] = NewItem;
		WasStatusChangedByEquip(NewItem, true);
	}

	if (Equipments[0]->UseSound)
	{
		UGameplayStatics::PlaySound2D(this, Equipments[0]->UseSound);
	}
}

void AMainPlayer::WasStatusChangedByEquip(AItem* Item, bool IsEquip)
{
	if (IsEquip)
	{
		IncDamage += Item->ItemTableValue.Damage;
		IncDeffence += Item->ItemTableValue.Deffence;
		IncStrength += Item->ItemTableValue.Strength;
		IncDexterity += Item->ItemTableValue.Dexterity;
		IncIntelligence += Item->ItemTableValue.Intelligence;
	}
	
	else
	{
		IncDamage -= Item->ItemTableValue.Damage;
		IncDeffence -= Item->ItemTableValue.Deffence;
		IncStrength -= Item->ItemTableValue.Strength;
		IncDexterity -= Item->ItemTableValue.Dexterity;
		IncIntelligence -= Item->ItemTableValue.Intelligence;
	}
}

int AMainPlayer::GetEquipmentIndex(AItem* Item)
{
	switch (Item->ItemTableValue.ItemType)
	{
	case EItemType::EIT_Weapon:
		return 0;

	case EItemType::EIT_Shield:
		return 1;

	case EItemType::EIT_Helmet:
		return 2;

	case EItemType::EIT_Chest:
		return 3;

	case EItemType::EIT_Shoulder:
		return 4;

	case EItemType::EIT_Glove:
		return 5;

	case EItemType::EIT_Pants:
		return 6;

	case EItemType::EIT_Boots:
		return 7;

	case EItemType::EIT_Ring:
		return 8;

	default:
		return -1;
	}
}

void AMainPlayer::UnEquipItem(AItem* Item)
{

}

#pragma endregion


void AMainPlayer::PlayMontage(FName Name, float PlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, PlayRate); // �ش� ��Ÿ�긦 n�� ������ ����Ѵ�.
		AnimInstance->Montage_JumpToSection(Name, CombatMontage);
	}
}

