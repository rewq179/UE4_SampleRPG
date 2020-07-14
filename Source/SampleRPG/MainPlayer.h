// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "Monster.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType) // �ش� Enum�� ����ϱ� ���ؼ� ǥ��������Ѵ�.
enum class EMovementState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"), // DisplayName("") : ��������Ϳ��� ""�� �̸����� �ش� ������ ǥ�����ְڴ�.
	EMS_Sprint UMETA(DisplayName = "Sprint"),
	EMS_Roll UMETA(DisplayName = "Roll"),

	EMS_MAX
};

USTRUCT(BlueprintType)
struct FPlayerStatusTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 CurExp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 MaxExp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	float CurStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	float Deffence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	int32 Dexterity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Status")
	FPlayerStatusTable Status;

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

	FORCEINLINE float GetTotalDamage() { return IncDamage + Status.Damage; }

	void TakeDamage(float Damage);
	void Death();
	void AddExp(int32 Exp);
	void CheckLevelUP();
	void SetLevelStatus(int32 CurLevel);
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Equipment")
	AItem* RecentItem;

	void AddItem(AItem* Item);
	void RemoveItem(AItem* Item);

	void EquipItem(AItem* NewItem);
	void UnEquipItem(AItem* Item);
	void WasStatusChangedByEquip(AItem* Item, bool IsEquip);

	int GetEquipmentIndex(AItem* Item);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Equipment")
	int32 Gold;

	FORCEINLINE void AddGold(int32 Value) { Gold += Value; }
	FORCEINLINE void RemoveGold(int32 Value) { Gold -= Value; }

#pragma endregion

#pragma region Combat
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MainCharacter|Combat")
	//class ACombatManager* CombatManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Combat")
	class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	TArray<AMonster*> TargetMonsters;

	FORCEINLINE void AddTargetMonster(AMonster* Monster) { TargetMonsters.AddUnique(Monster); }
	FORCEINLINE void RemoveTargetMonster(AMonster* Monster) { TargetMonsters.Remove(Monster); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	bool bIsAttackAnim; // ���� �ִϸ��̼��� �������ΰ�?
	void PlayAttackAnim(); // ���� �ִϸ��̼�(��Ÿ��) ��� ����

	UFUNCTION(BlueprintCallable)
	void AttackStart(); // �ִϸ��̼� ���� ����(�ִϸ��̼� ��� bool ���� true, ������ CombatCollision Ȱ��ȭ)
	UFUNCTION(BlueprintCallable)
	void AttackDamage(); // ������ �ִ� ����
	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // �ִϸ��̼� ���� ����(�ִϸ��̼� ��� bool ���� false, ������ CombatCollision ��Ȱ��ȭ)
	
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

	bool bIsLeftClickDown;
	void LeftClickDown(); // ���콺 ��Ŭ��
	void LeftClickUp();

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Input")
	bool bIsRoll;

	void Roll();

	UFUNCTION(BlueprintCallable)
	void RollEnd();

	void PlayMontage(FName Name, float PlayRate);


};
