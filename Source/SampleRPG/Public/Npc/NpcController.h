// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NpcController.generated.h"

UCLASS()
class SAMPLERPG_API ANpcController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANpcController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Npc|Properties")
	int32 NpcID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|Properties")
	class AItemManager* ItemManager;

	UPROPERTY(EditDefaultsOnly, Category = "Npc|Properties")
	TSubclassOf<class AItemManager> ItemManagerBP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc|Properties")
	class USphereComponent* InteractCollision; // 영역에 들어왔는지 체크

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Npc|Properties")
	class UAnimMontage* GestureMontage;

	UPROPERTY(VisibleAnywhere, Category = "Npc|Interact")
	class AMainPlayer* MainPlayer;


	class TArray<int32> SalesID;

	void C();

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
