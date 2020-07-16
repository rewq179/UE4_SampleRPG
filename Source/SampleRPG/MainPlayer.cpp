// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
		원리 : 셀카봉을 들고 뒤면 따라오듯이, 카메라를 스프링암에 고정시켜놓은다음 스프링암을 옮긴다.
	*/

	// 헤더파일 : "GameFramework/SpringArmComponent.h"
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bAbsoluteRotation = true; // 스프링 암의 회전이 루트 컴포넌트와 상위 컴포넌트를 따르지 않고 월드 좌표계의 회전을 따르도록 한다.
	CameraBoom->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	CameraBoom->TargetArmLength = 1000.f; // 셀카봉 길이는 어느정도로?
	CameraBoom->bDoCollisionTest = false; // 카메라가 벽을 뚫지 못하도록 만든다.

	//CameraBoom->bUsePawnControlRotation = true; // 카메라붐의 로테이션을 허용함

	// 헤더파일 : #include "Camera/CameraComponent.h"
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 카메라에따라 캐릭터를 움직이지 않겠다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// 헤더파일 : #include "GameFramework/CharacterMovementComponent.h"
	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터의 이동방향과 시야방향이 다르면 초당 640도의 회전 속도로 회전시킨다.
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
	StaminaRate = 15.f;

	RollCost = 50.f;
#pragma endregion

	// #include "UObject/ConstructorHelpers.h"

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/PlayerStatusTable.PlayerStatusTable'"));

	if (DataTable.Succeeded())
	{
		PlayerStatusTable = DataTable.Object;
	}

	Equipments.Init(nullptr, 10); 

	InterpSpeed = 15.f;
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

	IncreaseStamina(DeltaTime);
	InterpToMonster(DeltaTime);
}

void AMainPlayer::IncreaseStamina(float DeltaTime)
{
	if (Status.CurStamina <= Status.MaxStamina)
	{
		Status.CurStamina += DeltaTime * StaminaRate;
	}

	else
	{
		Status.CurStamina = Status.MaxStamina;
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // PlayerInputComponent 유무 체크

	// 헤더파일 : #include "GameFramework/PlayerController.h"

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

#pragma region Movement/Roll Function
void AMainPlayer::MoveForward(float Value)
{
	if (Value != 0.f && Controller && !bIsAttackAnim && MovementState != EMovementState::EMS_Dead)
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
	if (Value != 0.f && Controller && !bIsAttackAnim && MovementState != EMovementState::EMS_Dead)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::SetMovementState(EMovementState State) // 캐릭터상태에따라 최대속도가 변화됨
{
	MovementState = State;

	// 헤더파일 : #include "GameFramework/PlayerController.h"

	if (MovementState == EMovementState::EMS_Sprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // 최대 속도를 스프린트 속도로
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // 최대 속도를 일반 달리기 속도로
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
	if (!bIsRoll && MovementState != EMovementState::EMS_Dead && Status.CurStamina >= RollCost )
	{
		Status.CurStamina -= RollCost;
		bIsRoll = true;

		PlayMontage(FName("Roll"), 1.f);
	}
}

void AMainPlayer::RollEnd()
{
	bIsRoll = false;
}
#pragma endregion

#pragma region Status Function
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

float AMainPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (bIsRoll)
	{
		UE_LOG(LogTemp, Log, TEXT("ROLL!!!"));

		return 0.f;
	}

	float total = DamageAmount - Status.Deffence;

	if (total > 0)
	{
		Status.CurHP -= total;

		if (Status.CurHP <= 0)
		{
			Status.CurHP = 0;
			Death();
		}

		if (DamagedSound)
		{
			UGameplayStatics::PlaySound2D(this, DamagedSound, 0.5f);

		}
	}

	return total;
}

void AMainPlayer::Death()
{
	SetMovementState(EMovementState::EMS_Dead);

	PlayMontage(FName("Death"), 0.7f);
}

void AMainPlayer::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMainPlayer::Revive()
{
	Status.CurHP = Status.MaxHP;

	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
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

#pragma region Attack Function

void AMainPlayer::PlayAttackAnim()
{
	// 헤더파일 : #include "Engine/SkeletalMeshSocket.h"

	PlayMontage(FName("Attack_1"), 2.f);

	bIsInterp = true;
}

void AMainPlayer::AttackDamage()
{
	if (TargetMonsters.Num() > 0)
	{
		TargetMonster = TargetMonsters[0];

		for (auto Monster : TargetMonsters)
		{
			if (DamageType)
			{
				UGameplayStatics::ApplyDamage(Monster, Status.Damage, GetController(), this, DamageType);
			}

			//Monster->TakeDamage(GetTotalDamage());
		}

		if (Equipments[0]->AttackSound)
		{
			UGameplayStatics::PlaySound2D(this, Equipments[0]->AttackSound);
		}
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
	bIsInterp = false;

	Equipments[0]->SetCombatCollisionEnabled(false);
	TargetMonsters.Empty();

}
#pragma endregion

#pragma region Inventory Function
void AMainPlayer::AddItem(AItem* Item) 
{
	/*
		원리 :  인벤토리를 훑어서 아이템이 존재하는지 확인한다.

		if) O : Item의 Count를 증가한다. Count가 Max를 넘어가면 새로운 공간에 Item을 배치한다.
		if) X : 새로운 공간에 해당 Item을 배치한다.
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
	int32 SlotIndex = Inventory.IndexOfByKey(Item);

	int32 TotalCount = Inventory[SlotIndex]->Count -= Item->Count;

	if (TotalCount > 0)
	{
		Inventory[SlotIndex]->Count -= Item->Count;
	}

	else
	{
		Inventory.RemoveAt(SlotIndex);
	}
}

void AMainPlayer::EquipItem(AItem* NewItem)
{
	/*
		0:무기, 1:방패, 2:헬멧, 3:갑옷, 4:어깨방어구, 5:장갑, 6:하의, 7:신발, 8:악세 1, 9:악세 2
	*/

	int slotIndex = GetEquipmentIndex(NewItem);

	// 헤더파일 : #include "Engine/SkeletalMeshSocket.h"

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

void AMainPlayer::InterpToMonster(float DeltaTime)
{
	if (bIsInterp && TargetMonsters.Num() > 0)
	{
		FRotator DirectionYaw = GetTargetDirection(TargetMonsters[0]->GetActorLocation());
		FRotator InterpRotaion = FMath::RInterpTo(GetActorRotation(), DirectionYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotaion);
	}
}

FRotator AMainPlayer::GetTargetDirection(FVector Target)
{
	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator DirectionYaw(0.f,Direction.Yaw ,0.f);

	return DirectionYaw;
}

void AMainPlayer::UpdateWidgetMonster()
{
	if (WidgetMonsters.Num() == 0)
	{
		TargetMonster = nullptr;

		return;
	}

	for (int32 i = 0; i < WidgetMonsters.Num() - 1; i++)
	{
		int32 MinIndex = i;

		for (int32 j = i + 1; j < WidgetMonsters.Num(); j++)
		{
			if (GetPriorityByClass(WidgetMonsters[MinIndex]->Status.MonsterClass) < GetPriorityByClass(WidgetMonsters[j]->Status.MonsterClass))
			{
				MinIndex = j;
			}
		}

		WidgetMonsters.Swap(i, MinIndex);
	}

	TargetMonster = WidgetMonsters[0];
}

int32 AMainPlayer::GetPriorityByClass(EMonsterClass Class)
{
	switch (Class)
	{
	case EMonsterClass::EMC_Normal:
		return 1;

	case EMonsterClass::EMC_Elite:
		return 2;
	
	case EMonsterClass::EMC_Boss:
		return 3;

	default:
		return -1;
	}
}

void AMainPlayer::PlayMontage(FName Name, float PlayRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, PlayRate); // 해당 몽타쥬를 n배 빠르게 재상한다.
		AnimInstance->Montage_JumpToSection(Name, CombatMontage);
	}
}

