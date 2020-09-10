// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/DataTableManager.h"

#include "NpcController.generated.h"

/**
 * NPC의 DataTable, 퀘스트 Symbol 등을 저장하는 Npc 클래스
*/

UCLASS()
class SAMPLERPG_API ANpcController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANpcController();

	// Manager //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|ManagerClass")
	class AGameManager* GameManager;

	// Properties //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|Properties")
	class USphereComponent* InteractCollision; // 영역에 들어왔는지 체크

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Npc|Properties")
	class UStaticMeshComponent* QuestionSymbol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Npc|Properties")
	class UStaticMeshComponent* ExclamationSymbol;

	// DataTable //

	UPROPERTY(EditAnywhere, Category = "Npc|DataTable")
	int32 NpcID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	FNpcTable NpcData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> DialogueID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> ItemID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> QuestID;

	// Interact //

	UPROPERTY(VisibleAnywhere, Category = "Npc|Interact")
	class AMainPlayer* MainPlayer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Setter //
	
	void SetNpcData();
	void StringToIntArray(int32 Type, FString Data);
	void SetActiveSymbol(ESymbolType SymbolType);

	// Overlap //

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
