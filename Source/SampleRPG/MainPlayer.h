// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType) // 해당 Enum을 사용하기 위해선 표시해줘야한다.
enum class EMovementState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"), // DisplayName("") : 블루프린터에서 ""의 이름으로 해당 변수를 표시해주겠다.
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
	float NormalSpeed; // 일반 달리기 Max 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|Status")
	float SprintSpeed; // 스피린트 Max 속도

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
		원리 : 카메라를 고정 해놓은 SpringArm이 나를 따라 움직여서 나를 계속 보도록 만든다.
		meta = (AllowPrivateAccess = "true") : private 변수도 블루프린트 스크립트에서 활용이 가능해진다.
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom; // 셀카봉

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera; // 카메라

	FVector2D CameraInputValue; // 마우스 이동에 따른 값
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

	void LeftClickDown(); // 마우스 좌클릭

	void MoveForward(float Value);
	void MoveRight(float Value);
};
