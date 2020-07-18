// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "Monster.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType) // 해당 Enum을 사용하기 위해선 표시해줘야한다.
enum class EMovementState : uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"), // DisplayName("") : 블루프린터에서 ""의 이름으로 해당 변수를 표시해주겠다.
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
	float NormalSpeed; // 일반 달리기 Max 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float SprintSpeed; // 스피린트 Max 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacter|State")
	float StaminaRate; // 스테미나 증가량

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
	void InterpToMonster(float DeltaTime); // 캐릭터가 공격대상을 바라보게함
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

#pragma region Inventory/Equipments
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	TArray<AItem*> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacter|Inventory")
	int32 Gold;

	FORCEINLINE void AddGold(int32 Value) { Gold += Value; }
	FORCEINLINE void RemoveGold(int32 Value) { Gold -= Value; }

	// 장착 아이템

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
	bool bIsAttackAnim; // 어택 애니메이션이 진행중인가?
	
	void PlayAttackAnim(); // 공격 애니메이션(몽타주) 재생 관리

	// 아래는 애니메이션 Notify와 연동된 함수들

	UFUNCTION(BlueprintCallable)
	void AttackStart(); // CombatCollision 활성화
	UFUNCTION(BlueprintCallable)
	void AttackDamage(); // 데미지 주는 시점
	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // CombatCollision 비활성화
	
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
	void LeftClickDown(); // 마우스 좌클릭
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
