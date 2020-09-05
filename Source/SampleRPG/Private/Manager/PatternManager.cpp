// Fill out your copyright notice in the Description page of Project Settings.

#include "PatternManager.h"
#include "Manager/GameManager.h"

#include "Pattern.h"

#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
APatternManager::APatternManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FPatternTable APatternManager::GetPatternData(int32 PatternID)
{
	auto Pattern = PatternMap[PatternID].GetDefaultObject();
	Pattern->PatternID = PatternID;

	SetPatternData(Pattern, PatternID);

	return Pattern->PatternData;
}

void APatternManager::SetPatternData(APattern* Pattern, int32 PatternID)
{
	FPatternTable* PatternTableRow = GameManager->DataTableManager->GetPatternTableData(PatternID);
	
	Pattern->PatternData.PatternID = (*PatternTableRow).PatternID;
	Pattern->PatternData.PatternClass = (*PatternTableRow).PatternClass;
	Pattern->PatternData.Name = (*PatternTableRow).Name;
	Pattern->PatternData.Count = (*PatternTableRow).Count;
	Pattern->PatternData.AttackType = (*PatternTableRow).AttackType;
	Pattern->PatternData.StatusType = (*PatternTableRow).StatusType;
	Pattern->PatternData.SkillID = (*PatternTableRow).SkillID;
	Pattern->PatternData.CurHP = (*PatternTableRow).CurHP;
	Pattern->PatternData.PerHP = (*PatternTableRow).PerHP;
	Pattern->PatternData.CurST = (*PatternTableRow).CurST;
	Pattern->PatternData.PerST = (*PatternTableRow).PerST;

	Pattern->PatternData.Radius = (*PatternTableRow).Radius;

	Pattern->PatternData.UseParticle = (*PatternTableRow).UseParticle;
	Pattern->UseParticle->SetTemplate((*PatternTableRow).UseParticle);

	Pattern->PatternData.NotifyFieldParticle = (*PatternTableRow).NotifyFieldParticle;
	Pattern->NotifyFieldParticle->SetTemplate((*PatternTableRow).NotifyFieldParticle);

	Pattern->PatternData.DamageFieldParticle = (*PatternTableRow).DamageFieldParticle;
	Pattern->DamageFieldParticle->SetTemplate((*PatternTableRow).DamageFieldParticle);
}

APattern* APatternManager::CreatePatternActor(int32 PatternID)
{
	auto Pattern = GetWorld()->SpawnActor<APattern>(PatternMap[PatternID]);

	Pattern->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SetPatternData(Pattern, PatternID);

	return Pattern;
}

