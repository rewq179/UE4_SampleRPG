// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CustomDataTable.generated.h"

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
	EIT_Armor UMETA(DisplayName = "Armor"),

	EIT_Potion UMETA(DisplayName = "Potion"),

	EIT_MAX
};

// 헤더파일 : #include "Engine/DataTable.h"

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	EItemClass ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	bool bIsDroppable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTable")
	bool bIsSellable = true;

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

UCLASS()
class SAMPLERPG_API ACustomDataTable : public AActor
{
	GENERATED_BODY()

};