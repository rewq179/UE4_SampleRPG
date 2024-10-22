﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CombatManager.h"

#include "DataTableManager.generated.h"

/**
* DataTable의 칼럼들과 Enum들을 모아놓은 클래스다. Get???DataTable을 통해 DataTable 값을 받을 수 있다.
*/

#pragma region Enum Etc...
UENUM(BlueprintType)
enum class EObjectType : uint8
{
	EOT_Item UMETA(DisplayName = "Item"),
	EOT_Npc UMETA(DisplayName = "Npc"),
	EOT_Quest UMETA(DisplayName = "Quest"),
	EOT_Monster UMETA(DisplayName = "Monster")
};

UENUM(BlueprintType)
enum class ESymbolType : uint8
{
	EST_None UMETA(DisplayName = "None"),
	EST_Question UMETA(DisplayName = "Question"),
	EST_Exclamation UMETA(DisplayName = "Exclamation")
};

UENUM(BlueprintType)
enum class EApplyType : uint8
{
	EAT_None UMETA(DisplayName = "None"),
	EAT_Set UMETA(DisplayName = "Set"),
	EAT_Normal UMETA(DisplayName = "Normal"),
	EAT_Percent UMETA(DisplayName = "Percent")
};

UENUM(BlueprintType)
enum class EOperatorType : uint8
{
	EOT_None UMETA(DisplayName = "None"),
	EOT_Add UMETA(DisplayName = "Add"),
	EOT_Subtract UMETA(DisplayName = "Substract"),
	EOT_Multiply UMETA(DisplayName = "Multiply"),
	EOT_Devide UMETA(DisplayName = "Devide")
};

UENUM(BlueprintType)
enum class EHandType : uint8
{
	EHT_None UMETA(DisplayName = "None"),
	EHT_Left UMETA(DisplayName = "Left"),
	EHT_Right UMETA(DisplayName = "Right"),
	EHT_Both UMETA(DisplayName = "Both")
};

#pragma endregion

#pragma region NPC Table

UENUM(BlueprintType)
enum class ENpcClass : uint8
{
	ENC_None UMETA(DisplayName = "None"),
	ENC_Quest UMETA(DisplayName = "Quest"),
	ENC_Store UMETA(DisplayName = "Store"),
	ENC_Both UMETA(DisplayName = "Both")
};

USTRUCT(BlueprintType)
struct FNpcTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		int32 NpcID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		ENpcClass NpcClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NpcTable")
		FText Name;
	
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
	EMC_Boss UMETA(DisplayName = "Boss")

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
enum class EAttackClass : uint8
{
	EAC_Normal UMETA(DisplayName = "Normal"),
	EAC_Charging UMETA(DisplayName = "Charging"),
	EAC_Pattern UMETA(DisplayName = "Pattern"),
	EAC_Deffence UMETA(DisplayName = "Deffence")
};

UENUM(BlueprintType)
enum class EPatternClass : uint8
{
	EPT_None UMETA(DisplayName = "None"),
	EPT_Melee UMETA(DisplayName = "Melee"),
	EPT_Range UMETA(DisplayName = "Range"),
	EPT_Teleport UMETA(DisplayName = "Teleport")

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
		FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		EStatusType StatusType;
	
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
		float Radius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		UParticleSystem* UseParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		UParticleSystem* NotifyFieldParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatternTable")
		UParticleSystem* DamageFieldParticle;
};


#pragma endregion

#pragma region Skill Table & Enum Class
UENUM(BlueprintType)
enum class ESkillClass : uint8
{
	ESC_Active UMETA(DisplayName = "Active"),
	ESC_Buff UMETA(DisplayName = "Buff"),
	ESC_Debuff UMETA(DisplayName = "Debuff")
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
		EStatusType StatusType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTable")
		bool bIsSlow;

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
	EIC_Normal UMETA(DisplayName = "Normal")
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
	EIT_Normal UMETA(DisplayName = "Normal")
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
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		bool bIsPackaging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_0 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_0 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_0 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_1 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_2 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_3 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_3 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_3 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_4 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_4 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_4 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 ID_5 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Count_5 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent_5 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Pack_ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Pack_Percent = 0;

};


USTRUCT(BlueprintType)
struct FRewardTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 RewardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		bool bIsPackaging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		TArray<FRewardBox> Boxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		int32 Pack_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RewardTable")
		float Pack_Percent;
};
#pragma endregion

#pragma region Quest Table & Enum Class
UENUM(BlueprintType)
enum class EQuestClass : uint8
{
	EQC_Main UMETA(DisplayName = "Main"),
	EQC_Sub UMETA(DisplayName = "Sub"),
	EQC_Repeat UMETA(DisplayName = "Repeat")

};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	EQT_Kill UMETA(DisplayName = "Kill"),
	EQT_Collect UMETA(DisplayName = "Collect"),
	EQT_Use UMETA(DisplayName = "Use"),
	EQT_Equip UMETA(DisplayName = "Equip")

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

#pragma region Dialogue Table

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

#pragma endregion

#pragma region Trigger Table & Enum Class

// 조건을 만족하면 발동하는 시스템

UENUM(BlueprintType)
enum class ETriggerClass : uint8
{
	ETC_Instant UMETA(DisplayName = "Instant"),
	ETC_Check UMETA(DisplayName = "Check")
};

UENUM(BlueprintType)
enum class ETriggerType : uint8
{
	ETT_Block UMETA(DisplayName = "Block"),
	ETT_Spawn UMETA(DisplayName = "Spawn"),
	ETT_Clear UMETA(DisplayName = "Clear")
};

USTRUCT(BlueprintType)
struct FTriggerRawTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 TriggerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		ETriggerClass TriggerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		ETriggerType TriggerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		bool bOnGoing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		bool bIsClear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		FString BlockID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		FString MonsterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		FString PointID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 ID_0 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 CurCount_0 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 MaxCount_0 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 ID_1 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 CurCount_1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 MaxCount_1 = -1;
};

USTRUCT(BlueprintType)
struct FTriggerTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 TriggerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		ETriggerClass TriggerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		ETriggerType TriggerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		bool bOnGoing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		bool bIsClear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		TArray<int32> BlockIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		TArray<int32> MonsterIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		TArray<int32> PointIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 ID_0 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 CurCount_0 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 MaxCount_0 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 ID_1 = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 CurCount_1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTable")
		int32 MaxCount_1 = -1;
};
#pragma endregion

#pragma region DungeonRaw Table

USTRUCT(BlueprintType)
struct FDungeonRawTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		int32 DungeonID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FString TriggerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FString RewardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FString ItemID;
};

USTRUCT(BlueprintType)
struct FDungeonTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		int32 DungeonID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		TArray<int32> TriggerIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		TArray<int32> RewardIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueTable")
		TArray<int32> ItemIDs;
};

#pragma endregion

UCLASS()
class SAMPLERPG_API ADataTableManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADataTableManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DataTableManager|ManagerClass")
		class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DataTableManager|Properties")
		class AMainPlayer* MainPlayer;

	// 데이터 테이블 //
	class UDataTable* DialogueTableData;
	class UDataTable* DungeonRawTableData;
	class UDataTable* ItemTableData;
	class UDataTable* MonsterTableData;
	class UDataTable* NpcTableData;
	class UDataTable* PatternTableData;
	class UDataTable* QuestTableData;
	class UDataTable* RewardRawTableData; // RewardTable의 가공전 칼럼 => 가공후(ID_0, Count_0, Percent_0 => Box 구조체(BoxID, BoxCount, BoxPercent)
	class UDataTable* SkillTableData;
	class UDataTable* TriggerRawTableData;

public:

	// 데이터 테이블 Get() //

	FORCEINLINE FDialogueTable* GetDialogueTableData(int32 DialogueID) {
		return DialogueTableData->FindRow<FDialogueTable>(FName(*(FString::FormatAsNumber(DialogueID))), FString(""));
	}
	FORCEINLINE FDungeonRawTable* GetDungeonRawTableData(int32 DungeonRawID) {
		return DungeonRawTableData->FindRow<FDungeonRawTable>(FName(*(FString::FormatAsNumber(DungeonRawID))), FString(""));
	}
	FORCEINLINE FItemTable* GetItemTableData(int32 ItemID) {
		return ItemTableData->FindRow<FItemTable>(FName(*(FString::FormatAsNumber(ItemID))), FString(""));
	}
	FORCEINLINE FMonsterTable* GetMonsterTableData(int32 MonsterID) {
		return MonsterTableData->FindRow<FMonsterTable>(FName(*(FString::FormatAsNumber(MonsterID))), FString(""));
	}
	FORCEINLINE FNpcTable* GetNpcTableData(int32 NpcID) {
		return NpcTableData->FindRow<FNpcTable>(FName(*(FString::FormatAsNumber(NpcID))), FString(""));
	}
	FORCEINLINE FPatternTable* GetPatternTableData(int32 PatternID) {
		return PatternTableData->FindRow<FPatternTable>(FName(*(FString::FormatAsNumber(PatternID))), FString(""));
	}
	FORCEINLINE FQuestTable* GetQuestTableData(int32 QuestID) {
		return QuestTableData->FindRow<FQuestTable>(FName(*(FString::FormatAsNumber(QuestID))), FString(""));
	}
	FORCEINLINE FRewardRawTable* GetRewardRawTableData(int32 RewardID) {
		return RewardRawTableData->FindRow<FRewardRawTable>(FName(*(FString::FormatAsNumber(RewardID))), FString(""));
	}
	FORCEINLINE FSkillTable* GetSkillTableData(int32 SkillID) {
		return SkillTableData->FindRow<FSkillTable>(FName(*(FString::FormatAsNumber(SkillID))), FString(""));
	}
	FORCEINLINE FTriggerRawTable* GetTriggerRawTableData(int32 TriggerID) {
		return TriggerRawTableData->FindRow<FTriggerRawTable>(FName(*(FString::FormatAsNumber(TriggerID))), FString(""));
	}

	// Hud //

	UFUNCTION(BlueprintCallable)
		FText GetDialogueText(int32 DialogueID);

	UFUNCTION(BlueprintCallable)
		FText GetObjectName(EObjectType ObjectType, int32 ID);
};
