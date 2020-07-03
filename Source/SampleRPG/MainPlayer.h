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

	EMS_MAX
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

	void LeftClickDown(); // ���콺 ��Ŭ��

	void MoveForward(float Value);
	void MoveRight(float Value);
};
