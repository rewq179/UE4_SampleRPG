// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillManager.h"
#include "GameManager.h"

#include "Skill.h"

#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASkillManager::ASkillManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkillManager::BeginPlay()
{
	Super::BeginPlay();
	
}

ASkill* ASkillManager::CreateSkillActor(int32 SkillID)
{
	ASkill* Skill = GetWorld()->SpawnActor<ASkill>(SkillMap[SkillID]);

	Skill->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	SetSkillData(Skill, SkillID);

	return Skill;
}

void ASkillManager::SetSkillData(ASkill* Skill, int32 SkillID)
{
	FSkillRawTable* SkillRawTableRow = GameManager->DataTableManager->GetSkillRawTableData(SkillID);
	
	Skill->SkillData.SkillID = (*SkillRawTableRow).SkillID;
	Skill->SkillData.SkillClass = (*SkillRawTableRow).SkillClass;
	Skill->SkillData.SkillType = (*SkillRawTableRow).SkillType;
	Skill->SkillData.DamagedType = (*SkillRawTableRow).DamagedType;
	Skill->SkillData.Name = (*SkillRawTableRow).Name;
	Skill->SkillData.DurationTime = (*SkillRawTableRow).DurationTime;
	Skill->SkillData.CastTime = (*SkillRawTableRow).CastTime;
	Skill->SkillData.IncPerDamage = (*SkillRawTableRow).IncPerDamage;
	Skill->SkillData.CurHP = (*SkillRawTableRow).CurHP;
	Skill->SkillData.PerHP = (*SkillRawTableRow).PerHP;
	Skill->SkillData.CurST = (*SkillRawTableRow).CurST;
	Skill->SkillData.PerST = (*SkillRawTableRow).PerST;
	Skill->SkillData.PerShield = (*SkillRawTableRow).PerShield;

	Skill->SkillData.SkillShape = SkillShape((*SkillRawTableRow).Shape, (*SkillRawTableRow).X, (*SkillRawTableRow).Y);

	Skill->SkillData.UseParticle = (*SkillRawTableRow).UseParticle;
	Skill->UseParticle->SetTemplate((*SkillRawTableRow).UseParticle);

	Skill->SkillData.NotifyFieldParticle = (*SkillRawTableRow).NotifyFieldParticle;
	Skill->NotifyFieldParticle->SetTemplate((*SkillRawTableRow).NotifyFieldParticle);

	Skill->SkillData.DamageFieldParticle = (*SkillRawTableRow).DamageFieldParticle;
	Skill->DamageFieldParticle->SetTemplate((*SkillRawTableRow).DamageFieldParticle);

	Skill->SkillData.AnimationName = (*SkillRawTableRow).AnimationName;
	Skill->AnimationName = (*SkillRawTableRow).AnimationName;
}

FSkillShape ASkillManager::SkillShape(EShape Shape, float X, float Y)
{
	FSkillShape SkillShape;

	SkillShape.Shape = Shape;
	SkillShape.Size = FVector(X, Y, 0.f);

	return SkillShape;
}