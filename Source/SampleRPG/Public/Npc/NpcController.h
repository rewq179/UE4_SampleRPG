// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/DataTableManager.h"

#include "NpcController.generated.h"

UCLASS()
class SAMPLERPG_API ANpcController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANpcController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|Properties")
	class USphereComponent* InteractCollision; // 영역에 들어왔는지 체크

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Npc|Properties")
	class UAnimMontage* GestureMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Npc|Symbol")
	class UStaticMeshComponent* QuestionSymbol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Npc|Symbol")
	class UStaticMeshComponent* ExclamationSymbol;

	UPROPERTY(EditDefaultsOnly, Category = "Npc|DataTable")
	int32 NpcID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	FNpcTable NpcData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> DialogueID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> ItemID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|DataTable")
	TArray<int32> QuestID;

	UPROPERTY(VisibleAnywhere, Category = "Npc|Interact")
	class AMainPlayer* MainPlayer;

	void SetNpcData();

	void StringToIntArray(int32 Type, FString Data);

	void SetActiveSymbol(ESymbolType SymbolType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame

	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
