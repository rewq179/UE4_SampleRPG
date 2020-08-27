// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

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

UCLASS()
class SAMPLERPG_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

#pragma region Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class AGameManager* GameManager;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|Properties")
	TSubclassOf<class AInventory> InventoryBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class AInventory* Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|Properties")
	TSubclassOf<class APlayerCombat> PlayerCombatBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class APlayerCombat* PlayerCombat;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|Properties")
	TSubclassOf<class APlayerStatus> PlayerStatusBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class APlayerStatus* PlayerStatus;

	UPROPERTY(EditDefaultsOnly, Category = "MainCharacter|Properties")
	TSubclassOf<class APlayerQuest> PlayerQuestBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Properties")
	class APlayerQuest* PlayerQuest;
#pragma endregion

#pragma region State
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	EMovementState MovementState;

	void SetMovementState(EMovementState State);
	FORCEINLINE EMovementState GetMovementState() { return MovementState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float NormalSpeed; // �Ϲ� �޸��� Max �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float SprintSpeed; // ���Ǹ�Ʈ Max �ӵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|State")
	bool bIsEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|State")
	bool bIsAttackAnim; // ���� �ִϸ��̼��� �������ΰ�?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float RollCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	bool bIsMenuVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	bool bCanMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Input")
	bool bIsSprint; // Left Shift Key Down/Up

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Input")
	bool bIsRoll;

#pragma endregion

	/*
		���� : ī�޶� ���� �س��� SpringArm�� ���� ���� �������� ���� ��� ������ �����.
		meta = (AllowPrivateAccess = "true") : private ������ �������Ʈ ��ũ��Ʈ���� Ȱ���� ����������.
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; // ��ī��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera; // ī�޶�

	FVector2D CameraInputValue; // ���콺 �̵��� ���� ��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Interact")
	TArray<class AItem*> InteractItems;
	
	void AddItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Interact")
	class ANpcController* InteractNPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Interact")
	FName NextLevelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LeftShiftKeyDown();
	void LeftShiftKeyUp();

	bool bIsLeftClickDown;
	void LeftClickDown(); // ���콺 ��Ŭ��
	void LeftClickUp();

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void Roll();
	UFUNCTION(BlueprintCallable)
	void RollAnimEnd();


	void InteractObject();
	void StopCommunication();

	void SwitchLevel();
	bool IsLevelChange(FName NextLevelName);


	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	void LoadGameNoSwitch();
};
