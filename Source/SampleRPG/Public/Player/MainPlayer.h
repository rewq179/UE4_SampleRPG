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

	// �Ŵ��� Ŭ���� //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|ManagerClass")
	class AGameManager* GameManager;

	// ������Ʈ Ŭ���� //

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

	// �����Ʈ //

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
	bool bIsAttackAnim; // ���� �ִϸ��̼��� �������ΰ�?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPlayer|State")
	bool bIsMenuVisible;
	
	// ī�޶� //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; // ��ī��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera; // ī�޶�
	
	// ��ȣ�ۿ� //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	TArray<class AItem*> InteractItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	class ANpcController* InteractNPC;

	// �� �̸�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPlayer|Interact")
	FName NextLevelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
		// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitComponents();

	// ��ǲ �ƿ�ǲ Ű //
	
	void LeftClickDown(); // ���콺 ��Ŭ��

	// �����Ʈ �Լ��� // 

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void Roll();
	UFUNCTION(BlueprintCallable)
	void RollAnimEnd();

	// Interact �Լ��� //

	void InteractObject();
	void StopCommunication();

	void AddInteractedItemAll();
	void SwitchLevel();
	bool IsLevelChange(FName NextLevelName);

	// ���̺� �ε� //

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
};
