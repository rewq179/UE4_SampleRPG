// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MainPlayer.generated.h"

UCLASS()
class SAMPLERPG_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// 매니저 클래스 //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|ManagerClass")
	class AGameManager* GameManager;

	// 컴포넌트 클래스 //

	UPROPERTY(EditDefaultsOnly, Category = "MainPlayer|Components")
	TSubclassOf<class AInventory> InventoryBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Components")
	class AInventory* Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "MainPlayer|Components")
	TSubclassOf<class APlayerCombat> PlayerCombatBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Components")
	class APlayerCombat* PlayerCombat;

	UPROPERTY(EditDefaultsOnly, Category = "MainPlayer|Components")
	TSubclassOf<class APlayerStatus> PlayerStatusBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Components")
	class APlayerStatus* PlayerStatus;

	UPROPERTY(EditDefaultsOnly, Category = "MainPlayer|Components")
	TSubclassOf<class APlayerQuest> PlayerQuestBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Components")
	class APlayerQuest* PlayerQuest;

	// 무브먼트 //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	float NormalSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	float RollCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bCanMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsRoll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsAttackAnim; // 어택 애니메이션이 진행중인가?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPlayer|State")
	bool bIsMenuVisible;
	
	// 카메라 //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; // 셀카봉

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera; // 카메라
	
	// 상호작용 //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	TArray<class AItem*> InteractItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	class ANpcController* InteractNPC;

	// 맵 이름

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	FName NextLevelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
		// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitComponents();

	// 인풋 아웃풋 키 //
	
	void LeftClickDown(); // 마우스 좌클릭

	// 무브먼트 함수들 // 

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void Roll();
	UFUNCTION(BlueprintCallable)
	void RollAnimEnd();

	// Interact 함수들 //

	void InteractObject();
	void StopCommunication();

	void AddInteractedItemAll();
	void SwitchLevel();
	bool IsLevelChange(FName NextLevelName);

	// 세이브 로드 //

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
};
