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
	int32 ItemID; // 해당 ID로 아이템 데이터 테이블의 정보를 가져올 것이다.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	int32 Count; // 현재 개수

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
	class USphereComponent* InteractCollision; // 영역에 들어왔는지 체크

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Particle")
	class UParticleSystemComponent* IdleParticle; // 평소 파티클

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particle")
	class UParticleSystem* InteractParticle; // 반응시 보여줄 파티클

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* InteractSound; // 반응시 들려줄 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* UseSound; // 사용시 들려줄 사운드

	
		
#pragma region Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Collision")
	class UBoxComponent* CombatCollision; // 데미지 영역

	void SetCombatCollisionEnabled(bool IsActive);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* AttackSound;


#pragma endregion


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma region Weapon Bottom Public
	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void SetActiveText(bool bIsActive);

	void IgnoreStaticMesh();
#pragma endregion

};
