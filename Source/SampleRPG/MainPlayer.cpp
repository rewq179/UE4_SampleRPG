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

#include "RPGSaveGame.h"
#include "Animation/AnimInstance.h"
#include "ItemStorage.h"

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

	if (ItemStorageBP)
	{
		ItemStorage = GetWorld()->SpawnActor<AItemStorage>(ItemStorageBP);
	}

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
	if (Status.CurStamina < Status.MaxStamina)
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

void AMainPlayer::AdjustHP(float Amount, bool CanDie)
{
	Status.CurHP += Amount;

	if (Status.CurHP > Status.MaxHP)
	{
		Status.CurHP = Status.MaxHP;
	}

	if (Status.CurHP <= 0)
	{
		if (CanDie)
		{
			Death();
		}

		else
		{
			Status.CurHP = 1.f;
		}
	}
}

float AMainPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (bIsRoll)
	{
		return 0.f;
	}

	float total = DamageAmount - GetTotalDeffence();

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
		RecentItem->ItemOwner = this;
		
		AddItem(RecentItem);
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
	int32 InvIndex = GetInventoryIndex(Item);
	int32 TotalCount = Item->Count;
	int32 MaxCount = Item->ItemTableValue.MaxCount;

	if (InvIndex != -1) // 인벤토리에 아이템이 존재한다
	{
		TotalCount += Inventory[InvIndex]->Count;

		if (TotalCount > MaxCount)
		{
			Inventory[InvIndex]->Count = MaxCount;
			TotalCount -= MaxCount;
			
			DevideItemCount(Item, TotalCount);
		}

		else
		{
			Inventory[InvIndex]->Count += Item->Count;
		}
	}

	else // 인벤토리에 아이템이 없다
	{
		if (Item->Count > MaxCount)
		{
			Item->Count = MaxCount;
			TotalCount -= MaxCount;
			Inventory.Add(Item);

			DevideItemCount(Item, TotalCount);
		}

		else
		{
			Inventory.Add(Item);
		}
	}


	if (Item->ItemTableValue.ItemClass == EItemClass::EIC_Equip)
	{
		Item->IgnoreStaticMesh();
		Item->SetCombatCollisionEnabled(false);
	}
	
	Item->ItemOwner = this;
	Item->SetActorLocation(FVector(0.f));

	UpdateInventorySlot();
}

int32 AMainPlayer::GetInventoryIndex(AItem* Item)
{
	for (int32 InvIndex = 0; InvIndex != Inventory.Num(); InvIndex++) // 해당 아이템이 있나?
	{
		if (Inventory[InvIndex]->ItemID == Item->ItemID)
		{
			if (Inventory[InvIndex]->Count < Item->ItemTableValue.MaxCount)
			{
				return InvIndex;
			}
		}
	}

	return -1;
}

void AMainPlayer::DevideItemCount(AItem* Item, int32 TotalCount)
{
	while (TotalCount > 0)
	{
		AItem* DevideItem = GetItem(Item->ItemID);

		if (TotalCount > Item->ItemTableValue.MaxCount)
		{
			DevideItem->Count = Item->ItemTableValue.MaxCount;
		}

		else
		{
			DevideItem->Count = TotalCount;
		}

		Inventory.Add(DevideItem);
		TotalCount -= Item->ItemTableValue.MaxCount;
	}
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

void AMainPlayer::UseItem(AItem* Item, int32 SlotIndex)
{
	if (Item->Count > 0)
	{
		if (Item->ItemTableValue.ItemClass == EItemClass::EIC_Equip)
		{
			EquipItem(Item, SlotIndex);
		}

		else if (Item->ItemTableValue.ItemClass == EItemClass::EIC_Consume)
		{
			ConsumeItem(Item, SlotIndex);
		}
	}
}

void AMainPlayer::ConsumeItem(AItem* Item, int32 SlotIndex)
{
	Inventory[SlotIndex]->Count--;

	AdjustHP(Item->ItemTableValue.Damage, false);

	if (Item->UseSound)
	{
		UGameplayStatics::PlaySound2D(this, Item->UseSound);
	}
}

void AMainPlayer::EquipItem(AItem* NewItem, int32 InvIndex)
{
	/*
		0:무기, 1:방패, 2:헬멧, 3:갑옷, 4:어깨방어구, 5:장갑, 6:하의, 7:신발, 8:악세 1, 9:악세 2
	*/
	Inventory.RemoveAt(InvIndex);

	int32 EquipIndex = GetEquipmentIndex(NewItem);

	// 헤더파일 : #include "Engine/SkeletalMeshSocket.h"

	if (EquipIndex == 0) // 무기
	{
		const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");

		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(NewItem, GetMesh());
		}
	}

	else if (EquipIndex == 1) // 방패
	{
		const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName("LeftHandSocket");

		if (LeftHandSocket)
		{
			LeftHandSocket->AttachActor(NewItem, GetMesh());
		}
	}

	else if (EquipIndex == 8) // 반지
	{
		if (Equipments[8] && !Equipments[9])
		{
			EquipIndex++;
		}
	}

	if (Equipments[EquipIndex])
	{
		UnEquipItem(EquipIndex);
	}

	Equipments[EquipIndex] = NewItem;
	WasStatusChangedByEquip(NewItem, true);

	if (Equipments[EquipIndex]->UseSound)
	{
		UGameplayStatics::PlaySound2D(this, Equipments[EquipIndex]->UseSound);
	}
}

void AMainPlayer::UnEquipItem(int32 SlotIndex)
{
	WasStatusChangedByEquip(Equipments[SlotIndex], false);
	AddItem(Equipments[SlotIndex]);
	Equipments[SlotIndex] = nullptr;
}

int32 AMainPlayer::GetEquipmentIndex(AItem* Item)
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
	URPGSaveGame* SaveGameInstance =  Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));

	SaveGameInstance->PlayerStatus.Level = Status.Level;
	SaveGameInstance->PlayerStatus.CurExp = Status.CurExp;
	SaveGameInstance->PlayerStatus.MaxExp = Status.MaxExp;
	SaveGameInstance->PlayerStatus.CurHP = Status.CurHP;
	SaveGameInstance->PlayerStatus.MaxHP = Status.MaxHP;
	SaveGameInstance->PlayerStatus.CurStamina = Status.CurStamina;
	SaveGameInstance->PlayerStatus.MaxStamina = Status.MaxStamina;
	SaveGameInstance->PlayerStatus.Damage = Status.Damage;
	SaveGameInstance->PlayerStatus.Deffence = Status.Deffence;
	SaveGameInstance->PlayerStatus.Strength = Status.Strength;
	SaveGameInstance->PlayerStatus.Dexterity = Status.Dexterity;
	SaveGameInstance->PlayerStatus.Intelligence = Status.Intelligence;

	SaveGameInstance->Location = GetActorLocation();
	SaveGameInstance->Rotation = GetActorRotation();

	for (auto Item : Inventory)
	{
		if (SaveGameInstance->InventoryItemMap.Contains(Item->ItemID))
		{
			int32 Key = GetItemKey(SaveGameInstance->InventoryItemMap, Item->ItemID);
			int32 Value = SaveGameInstance->InventoryItemMap[Key];
			
			SaveGameInstance->InventoryItemMap.Emplace(Item->ItemID, Value + Item->Count);
		}

		else
		{
			SaveGameInstance->InventoryItemMap.Add(Item->ItemID, Item->Count);
		}
	}

	for (int32 SlotIndex = 0; SlotIndex < 10; SlotIndex++ )
	{
		if (Equipments[SlotIndex])
		{
			SaveGameInstance->EquipmentItem[SlotIndex] = Equipments[SlotIndex]->ItemID;
		}

		else
		{
			SaveGameInstance->EquipmentItem[SlotIndex] = -1;
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

int32 AMainPlayer::GetItemKey(TMap<int32, int32> Map, int32 ID)
{
	int32 Key = -1;

	for (auto& Item : Map)
	{
		if (Item.Key == ID)
		{
			Key = Item.Key;
		}
	}

	return Key;
}

void AMainPlayer::LoadGame(bool SetPosition)
{
	URPGSaveGame* LoadGameInstance = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));

	LoadGameInstance = Cast<URPGSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	Status.Level = LoadGameInstance->PlayerStatus.Level;
	Status.CurExp = LoadGameInstance->PlayerStatus.CurExp;
	Status.MaxExp = LoadGameInstance->PlayerStatus.MaxExp;
	Status.CurHP = LoadGameInstance->PlayerStatus.CurHP;
	Status.MaxHP = LoadGameInstance->PlayerStatus.MaxHP;
	Status.CurStamina = LoadGameInstance->PlayerStatus.CurStamina;
	Status.MaxStamina = LoadGameInstance->PlayerStatus.MaxStamina;
	Status.Damage = LoadGameInstance->PlayerStatus.Damage;
	Status.Deffence = LoadGameInstance->PlayerStatus.Deffence;
	Status.Strength = LoadGameInstance->PlayerStatus.Strength;
	Status.Dexterity = LoadGameInstance->PlayerStatus.Dexterity;
	Status.Intelligence = LoadGameInstance->PlayerStatus.Intelligence;

	if (SetPosition)
	{
		SetActorLocation(LoadGameInstance->Location);
		SetActorRotation(LoadGameInstance->Rotation);
	}

	if (ItemStorageBP && ItemStorage)
	{
		Inventory.Empty();

		for (auto InvItem : LoadGameInstance->InventoryItemMap)
		{
			AItem* Item = GetItem(InvItem.Key);
			Item->Count = InvItem.Value;

			AddItem(Item);
		}

		for (int32 SlotIndex = 0; SlotIndex < 10; SlotIndex++)
		{
			if (LoadGameInstance->EquipmentItem[SlotIndex] >= 0)
			{
				AItem* Item = GetItem(LoadGameInstance->EquipmentItem[SlotIndex]);
				Item->Count = 1;

				AddItem(Item);
				EquipItem(Item, SlotIndex);
			}
		}
	}
}

AItem* AMainPlayer::GetItem(int32 ItemID)
{
	AItem* Item = GetWorld()->SpawnActor<AItem>(ItemStorage->ItemMap[ItemID]);

	return Item;
}
