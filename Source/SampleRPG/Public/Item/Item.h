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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Properties")
	class AGameManager* GameManager;

#pragma region ItemTable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	int32 ItemID; // �ش� ID�� ������ ������ ���̺��� ������ ������ ���̴�.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	int32 Count; // ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	FItemTable ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	USkeletalMeshComponent* EquipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	UTexture2D* Icon;

	virtual void SetItemData();

#pragma endregion

	UPROPERTY(VisibleAnywhere, Category = "Item|State")
	EItemState ItemState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|State")
	class AMainPlayer* ItemOwner;

	FORCEINLINE void SetItemState(EItemState State) { ItemState = State; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Collision")
	class USphereComponent* InteractCollision; // ������ ���Դ��� üũ

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Particle")
	class UParticleSystemComponent* IdleParticle; // ��� ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particle")
	class UParticleSystem* InteractParticle; // ������ ������ ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* InteractSound; // ������ ����� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* UseSound; // ���� ����� ����

	
		
#pragma region Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Collision")
	class UBoxComponent* CombatCollision; // ������ ����

	void SetCombatCollisionEnabled(bool IsActive);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* AttackSound;


#pragma endregion


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION() // ���� �ڽ� Ŭ�������� �������ϸ� UFUNCTION()�� �����ؾ��Ѵ�.
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma region Weapon Bottom Public
	UFUNCTION() // ���� �ڽ� Ŭ�������� �������ϸ� UFUNCTION()�� �����ؾ��Ѵ�.
	void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveText(bool bIsActive);

	void IgnoreStaticMesh();
#pragma endregion

};
