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
	EMS_Dead UMETA(DisplayName = "Dead"),
	
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float NormalSpeed; // �Ϲ� �޸��� Max �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float SprintSpeed; // ���Ǹ�Ʈ Max �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float StaminaRate; // ���׹̳� ������

	void IncreaseStamina(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float RollCost;

	void SetMovementState(EMovementState State);
	FORCEINLINE EMovementState GetMovementState() { return MovementState; }

#pragma endregion

#pragma region Status

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

	bool bIsPlayerDead;
	void AdjustHP(float Amount, bool CanDie);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	void Death();
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	void Revive();
	void AddExp(int32 Exp);
	void CheckLevelUP();
	void SetLevelStatus(int32 CurLevel);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainCharacter|Combat")
	TSubclassOf<UDamageType> DamageType;
	
#pragma endregion

#pragma region Interp
	float InterpSpeed;
	bool bIsInterp;

	FRotator GetTargetDirection(FVector Target);
	void InterpToMonster(float DeltaTime); // ĳ���Ͱ� ���ݴ���� �ٶ󺸰���
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

#pragma region Inventory/Equipments
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	TArray<AItem*> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	int32 Gold;

	FORCEINLINE void AddGold(int32 Value) { Gold += Value; }
	FORCEINLINE void RemoveGold(int32 Value) { Gold -= Value; }

	// ���� ������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Equipment")
	TArray<AItem*> Equipments; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Equipment")
	AItem* RecentItem;

	void AddItem(AItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(AItem* Item);
	UFUNCTION(BlueprintCallable)
	void UseItem(AItem* Item, int32 SlotIndex);
	void ConsumeItem(AItem* Item, int32 SlotIndex);
	void EquipItem(AItem* NewItem, int32 SlotIndex);
	void UnEquipItem(AItem* Item, int32 SlotIndex);
	
	void WasStatusChangedByEquip(AItem* Item, bool IsEquip);
	
#pragma endregion

#pragma region Combat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class USoundCue* DamagedSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	TArray<AMonster*> TargetMonsters;

	FORCEINLINE void AddTargetMonster(AMonster* Monster) { TargetMonsters.AddUnique(Monster); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	TArray<AMonster*> WidgetMonsters;
	
	FORCEINLINE void AddWidgetMonster(AMonster* Monster) { WidgetMonsters.AddUnique(Monster); UpdateWidgetMonster();}
	FORCEINLINE void RemoveWidgetMonster(AMonster* Monster) { WidgetMonsters.Remove(Monster); UpdateWidgetMonster();}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	class AMonster* TargetMonster;
	
	void UpdateWidgetMonster();
	int32 GetPriorityByClass(EMonsterClass Class);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Combat")
	bool bIsAttackAnim; // ���� �ִϸ��̼��� �������ΰ�?
	
	void PlayAttackAnim(); // ���� �ִϸ��̼�(��Ÿ��) ��� ����

	// �Ʒ��� �ִϸ��̼� Notify�� ������ �Լ���

	UFUNCTION(BlueprintCallable)
	void AttackStart(); // CombatCollision Ȱ��ȭ
	UFUNCTION(BlueprintCallable)
	void AttackDamage(); // ������ �ִ� ����
	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // CombatCollision ��Ȱ��ȭ
	
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
