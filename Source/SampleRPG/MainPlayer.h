// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType) // �ش� Enum�� ����ϱ� ���ؼ� ǥ��������Ѵ�.
enum class EMovementState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"), // DisplayName("") : ��������Ϳ��� ""�� �̸����� �ش� ������ ǥ�����ְڴ�.
	EMS_Sprint UMETA(DisplayName = "Sprint"),

	EMS_MAX
};

USTRUCT(BlueprintType)
struct FPlayerStatusTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Exp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
	float CurHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
	float CurStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainCharacter|Status")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainCharacter|Status")
	float Deffence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainCharacter|Status")
	int32 Strength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainCharacter|Status")
	int32 Dexterity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainCharacter|Status")
	int32 Intelligence;
};


UCLASS()
class SAMPLERPG_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();


#pragma region MovementState
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
		EMovementState MovementState;

	void SetMovementState(EMovementState State);
	FORCEINLINE EMovementState SetMovementState() { return MovementState; }

#pragma endregion

#pragma region Status
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
	float NormalSpeed; // �Ϲ� �޸��� Max �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
	float SprintSpeed; // ���Ǹ�Ʈ Max �ӵ�

	class UDataTable* PlayerStatusTable;
	FPlayerStatusTable* PlayerStatusTableRow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 MaxExp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 CurExp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float CurHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float CurStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float Deffence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Strength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Dexterity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Intelligence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float IncDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	float IncDeffence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 IncStrength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 IncDexterity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 IncIntelligence;

	void TakeDamage(float Damage);
	void Death();
	void LevelUP();
	void SetLevelStatus(int CurLevel);
#pragma endregion

#pragma region Camera
	/*
		���� : ī�޶� ���� �س��� SpringArm�� ���� ���� �������� ���� ��� ������ �����.
		meta = (AllowPrivateAccess = "true") : private ������ �������Ʈ ��ũ��Ʈ���� Ȱ���� ����������.
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom; // ��ī��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera; // ī�޶�

	FVector2D CameraInputValue; // ���콺 �̵��� ���� ��
#pragma endregion

#pragma region Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	TArray<AItem*> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Equipment")
	TArray<AItem*> Equipments;

	void AddItem(AItem* Item);
	void RemoveItem(AItem* Item);

	void EquipItem(AItem* Item);
	void UnEquipItem(AItem* Item);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Equipment")
	int32 Gold;

	FORCEINLINE void AddGold(int32 Value) { Gold += Value; }
	FORCEINLINE void RemoveGold(int32 Value) { Gold -= Value; }

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Item")
	//	class AWeapon* EquipWeapon;

	//void SetEquipWeapon(AWeapon* Weapon);
	//FORCEINLINE AWeapon* GetEquipWeapon() { return EquipWeapon; }

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Item")
	//	class AItem* InteractItem;

	//FORCEINLINE void SetInteractItem(AItem* Item) { InteractItem = Item; }
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Input")
	bool bIsSprint; // Left Shift Key Down/Up

	void LeftShiftKeyDown();
	void LeftShiftKeyUp();

	void ToggleInventoryUI();
	void InteractObject();

	void LeftClickDown(); // ���콺 ��Ŭ��

	void MoveForward(float Value);
	void MoveRight(float Value);
};
