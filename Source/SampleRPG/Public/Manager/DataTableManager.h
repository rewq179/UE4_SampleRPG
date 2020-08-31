// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CombatManager.h"

#include "DataTableManager.generated.h"

#pragma region Enum Etc...
UENUM(BlueprintType)
enum class EObjectType : uint8
{
	EOT_Item UMETA(DisplayName = "Item"),
	EOT_Npc UMETA(DisplayName = "Npc"),
	EOT_Quest UMETA(DisplayName = "Quest"),
	EOT_Monster UMETA(DisplayName = "Monster"),

	EOT_Max
};

UENUM(BlueprintType)
enum class ESymbolType : uint8
{
	EST_None UMETA(DisplayName = "None"),
	EST_Question UMETA(DisplayName = "Question"),
	EST_Exclamation UMETA(DisplayName = "Exclamation"),

	EST_MAX
};

UENUM(BlueprintType)
enum class EApplyType : uint8
{
	EAT_None UMETA(DisplayName = "None"),
	EAT_Set UMETA(DisplayName = "Set"),
	EAT_Normal UMETA(DisplayName = "Normal"),
	EAT_Percent UMETA(DisplayName = "Percent"),

	EAT_MAX
};

UENUM(BlueprintType)
enum class EOperatorType : uint8
{
	EOT_None UMETA(DisplayName = "None"),
	EOT_Add UMETA(DisplayName = "Add"),
	EOT_Subtract UMETA(DisplayName = "Substract"),
	EOT_Multiply UMETA(DisplayName = "Multiply"),
	EOT_Devide UMETA(DisplayName = "Devide"),

	EOT_MAX
};

UENUM(BlueprintType)
enum class EShape : uint8
{
	ES_None UMETA(DisplayName = "None"),
	ES_Circle UMETA(DisplayName = "Circle"),
	ES_Rectangle UMETA(DisplayName = "Rectangle"),

	ES_MAX
};
#pragma endregion


#pragma region NPC Table
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		FString QuestID;
};
#pragma endregion

#pragma region Monster Table & Enum Class

UENUM(BlueprintType)
enum class EMonsterClass : uint8
{
	EMC_Normal UMETA(DisplayName = "Normal"),
	EMC_Elite UMETA(DisplayName = "Elite"),
	EMC_Boss UMETA(DisplayName = "Boss"),

	EMC_MAX
};
USTRUCT(BlueprintType)
struct FMonsterTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		EMonsterClass MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		int32 AttackCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		FString PatternID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		bool bHasCharging;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float CurGroggy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float MaxGroggy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float Deffence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float FollowSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float DetectRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		int32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		int32 ByProductID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterTable")
		int32 RewardID;
};

#pragma endregion

#pragma region Monster Pattern & Enum Class

UENUM(BlueprintType)
enum class EPatternClass : uint8
{
	EPC_Normal UMETA(DisplayName = "Normal"),
	EPC_Charging UMETA(DisplayName = "Charging"),
	EPC_Skill UMETA(DisplayName = "Skill"),
	EPC_Deffence UMETA(DisplayName = "Deffence"),

	EPC_MAX
};

UENUM(BlueprintType)
enum class EPatternType : uint8
{
	EPT_Realtime UMETA(DisplayName = "RealTime"),
	EPT_Duration UMETA(DisplayName = "Duration"),

	EPT_MAX
};

USTRUCT(BlueprintType)
struct FPatternShape : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternShape")
		EShape Shape;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternShape")
		FVector Size;
};

USTRUCT(BlueprintType)
struct FPatternRawTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		int32 PatternID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		EPatternClass PatternClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		EPatternType PatternType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		EDamagedType DamagedType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		int32 SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float PerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float CurST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float PerST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		EShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		float X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		float Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		UParticleSystem* UseParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		UParticleSystem* NotifyFieldParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		UParticleSystem* DamageFieldParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternRawTable")
		FString AnimationName;
};

USTRUCT(BlueprintType)
struct FPatternTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		int32 PatternID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		EPatternClass PatternClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		EPatternType PatternType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		EDamagedType DamagedType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		int32 SkillID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float PerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float CurST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		float PerST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		FPatternShape PatternShape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		UParticleSystem* UseParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		UParticleSystem* NotifyFieldParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		UParticleSystem* DamageFieldParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		FString AnimationName;
};

#pragma endregion

#pragma region Skill Table & Enum Class
UENUM(BlueprintType)
enum class ESkillClass : uint8
{
	ESC_Active UMETA(DisplayName = "Active"),
	ESC_Buff UMETA(DisplayName = "Buff"),
	ESC_Debbuf UMETA(DisplayName = "Debuff"),

	ESC_MAX
};

USTRUCT(BlueprintType)
struct FSkillTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		int32 SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		ESkillClass SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float DurationTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float IncPerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float IncPerDeffence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float PerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float CurST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float PerST;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float PerShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		float PerSpeed;
};
#pragma endregion


#pragma region Item Table & Enum Class

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
#pragma endregion

#pragma region Reward Table
USTRUCT(BlueprintType)
struct FRewardBox : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
	int32 BoxID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
	int32 BoxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
	float BoxPercent;
};

USTRUCT(BlueprintType)
struct FRewardRawTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 RewardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_5;
};


USTRUCT(BlueprintType)
struct FRewardTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 RewardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
	TArray<FRewardBox> Boxes;
};
#pragma endregion

#pragma region Quest Table & Enum Class
UENUM(BlueprintType)
enum class EQuestClass : uint8
{
	EQC_Main UMETA(DisplayName = "Main"),
	EQC_Sub UMETA(DisplayName = "Sub"),
	EQC_Repeat UMETA(DisplayName = "Repeat"),

	EQC_MAX
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	EQT_Kill UMETA(DisplayName = "Kill"),
	EQT_Collect UMETA(DisplayName = "Collect"),
	EQT_Use UMETA(DisplayName = "Use"),
	EQT_Equip UMETA(DisplayName = "Equip"),

	EQT_MAX
};

// 헤더파일 : #include "Engine/DataTable.h"

USTRUCT(BlueprintType)
struct FQuestTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 QuestSubID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		EQuestClass QuestClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		EQuestType QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		FText Goal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 PreDialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 PostDialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 PreQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		bool bIsLastQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		bool bIsAlreadyGive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		bool bCanClear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		bool bIsClear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 NpcID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 CurCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 PreReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 PreRewardCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 PostReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestTable")
		int32 PostRewardCount;
};
#pragma endregion

USTRUCT(BlueprintType)
struct FDialogueTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		int32 DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FText DialogueText;
};

UCLASS()
class SAMPLERPG_API ADataTableManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataTableManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTableManager|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataTableManager|DataTable")
	class AMainPlayer* MainPlayer;

	class UDataTable* DialogueTableData;
	FDialogueTable* GetDialogueTableData(int32 DialogueID);

	class UDataTable* NpcTableData;
	FNpcTable* GetNpcTableData(int32 NpcID);

	class UDataTable* MonsterTableData;
	FMonsterTable* GetMonsterTableData(int32 MonsterID);
	
	class UDataTable* PatternRawTableData;
	FPatternRawTable* GetPatternRawTableData(int32 PatternID);

	class UDataTable* ItemTableData;
	FItemTable* GetItemTableData(int32 ItemID);
	
	class UDataTable* RewardRawTableData;
	FRewardRawTable* GetRewardRawTableData(int32 RewardID);

	class UDataTable* QuestTableData;
	FQuestTable* GetQuestTableData(int32 QuestID);

	class UDataTable* SkillTableData;
	FSkillTable* GetSkillTableData(int32 SkillID);

	UFUNCTION(BlueprintCallable)
	FText GetDialogueText(int32 DialogueID);
	
	UFUNCTION(BlueprintCallable)
	FText GetObjectName(EObjectType ObjectType, int32 ID);
};
