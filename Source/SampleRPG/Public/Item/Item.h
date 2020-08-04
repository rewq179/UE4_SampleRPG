// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemClass : uint8
{
	EIC_Equip UMETA(DisplayName = "Equip"),
	EIC_Consume UMETA(DisplayName = "Consume"),
	EIC_Normal UMETA(DisplayName = "Normal"),

	EIC_MAX
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Shield UMETA(DisplayName = "Shield"),
	EIT_Helmet UMETA(DisplayName = "Helmet"),
	EIT_Chest UMETA(DisplayName = "Chest"),
	EIT_Glove UMETA(DisplayName = "Glove"),
	EIT_Shoulder UMETA(DisplayName = "Shoulder"),
	EIT_Pants UMETA(DisplayName = "Pants"),
	EIT_Boots UMETA(DisplayName = "Boots"),
	EIT_Ring UMETA(DisplayName = "Ring"),
	EIT_Potion UMETA(DisplayName = "Potion"),
	EIT_HoldPotion UMETA(DisplayName = "HoldPotion"),
	EIT_Normal UMETA(DisplayName = "Normal"),
	
	EIT_MAX
};

// 헤더파일 : #include "Engine/DataTable.h"

USTRUCT(BlueprintType)
struct FItemTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	EItemClass ItemClass = EItemClass::EIC_Equip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	EItemType ItemType = EItemType::EIT_Potion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	bool bIsDroppable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	bool bIsSellable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 BuyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 MaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	float Deffence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 Dexterity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 Intelligence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	float DurationTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	class USkeletalMesh* EquipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	UTexture2D* Icon;
};

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

#pragma region ItemTable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	int32 ItemID; // 해당 ID로 아이템 데이터 테이블의 정보를 가져올 것이다.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	int32 Count; // 현재 개수

	class UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	FItemTable ItemTableValue;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma region Weapon Bottom Public
	UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void IgnoreStaticMesh();
#pragma endregion

};
