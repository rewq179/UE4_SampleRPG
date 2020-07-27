// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"

#include "NpcController.generated.h"


USTRUCT(BlueprintType)
struct FNpcTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
	int32 NpcID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
	bool bHasItem = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
	bool bHasQuest = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
	FString DialogueID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
	FString ItemID;
};

UCLASS()
class SAMPLERPG_API ANpcController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANpcController();

	UPROPERTY(EditDefaultsOnly, Category = "Npc|Properties")
	TSubclassOf<class AItemManager> ItemManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|Properties")
	class AItemManager* ItemManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|Properties")
	class USphereComponent* InteractCollision; // 영역에 들어왔는지 체크

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Npc|Properties")
	class UAnimMontage* GestureMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Npc|DataTable")
	int32 NpcID;

	class UDataTable* NpcTable;
	FNpcTable* NpcTableRow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	FNpcTable Npc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> DialogueID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> ItemID;

	UPROPERTY(VisibleAnywhere, Category = "Npc|Interact")
	class AMainPlayer* MainPlayer;

	void StringToIntArray(bool bIsItem, FString Data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
