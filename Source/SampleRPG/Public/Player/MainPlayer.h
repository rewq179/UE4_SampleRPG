// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Player/PlayerStatus.h"

#include "MainPlayer.generated.h"

UCLASS()
class SAMPLERPG_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// Manager //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|ManagerClass")
	class AGameManager* GameManager;

	// Components //

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

	// Movement //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Movement")
	float NormalSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Movement")
	float RollCost;

	// State //

	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bCanMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsRoll;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bCanRollAttack;

	FORCEINLINE void SetRollAttack() { bCanRollAttack = false; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|State")
	bool bIsAttackAnim; // ���� �ִϸ��̼��� �������ΰ�?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPlayer|State")
	bool bIsMenuVisible; // UI�� On�Ǿ������� �������δ�.
	
	// Camera //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainPlayer|Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; // ��ī��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainPlayer|Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera; // ī�޶�
	
	// Interact Object //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	TArray<class AItem*> InteractItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	class ANpcController* InteractNPC;

	// Save & Load //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|SaveLoad")
	TMap<int32, FPlayerStatTable> PlayerStatMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|SaveLoad")
	FString CurSlotName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void InitComponents();

	// Input Key //

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void LeftClickDown(); // ���콺 ��Ŭ��

	// Movement // 

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void Roll();
	UFUNCTION(BlueprintCallable)
	void RollAnimEnd();
	
	// Interact //

	void InteractObject();

	void AddInteractedItemAll();

	// Save & Load GameData //

	UFUNCTION(BlueprintCallable)
	void SaveGame(FString SlotName);
	UFUNCTION(BlueprintCallable)
	void LoadGame(FString SlotName);


	UFUNCTION(BlueprintCallable)
	FPlayerStatTable LoadData(FString SlotName);

	void LoadLevel(bool bIsLobby);

	void SwitchLevel(FString LevelName);
};
