// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/DataTableManager.h"

#include "Item.generated.h"


UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Root UMETA(DisplayName = "NotRoot"),
	EIS_Inv UMETA(DisplayName = "Inventory"),

	EIS_MAX
};

UCLASS()
class SAMPLERPG_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|ManagerClass")
	class AGameManager* GameManager;

	// DataTable //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|DataTable")
	int32 ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|DataTable")
	int32 Count; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|DataTable")
	FItemTable ItemData;

	// Mesh // 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	USkeletalMeshComponent* EquipMesh;

	// Icon //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	UTexture2D* Icon;

	// Collision //

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Properties")
	class USphereComponent* InteractCollision; // ������ ���Դ��� üũ

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	class UBoxComponent* CombatCollision; // ������ ����

	// Particle //

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	class UParticleSystemComponent* IdleParticle; // ��� ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Properties")
	class UParticleSystem* InteractParticle; // ������ ������ ��ƼŬ

	// Sound //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Properties")
	class USoundCue* InteractSound; // ������ ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Properties")
	class USoundCue* UseSound; // ���� ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Properties")
	class USoundCue* AttackSound;

	// Item State //
	
	UPROPERTY(VisibleAnywhere, Category = "Item|State")
	EItemState ItemState;
	FORCEINLINE void SetItemState(EItemState State) { ItemState = State; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|State")
	class AMainPlayer* ItemOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void SetItemData();
	void SetItemOwner(class AMainPlayer* MainPlayer);

	void SetEnabledCombatCollision(bool IsActive);
	void IgnoreStaticMesh();


	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveText(bool bIsActive); // ������ �̸��� ���� ȭ�鿡 �����.

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
