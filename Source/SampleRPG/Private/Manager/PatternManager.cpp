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

// Called when the game starts or when spawned
void APatternManager::BeginPlay()
{
	Super::BeginPlay();
	
}

FPatternTable APatternManager::GetPatternData(int32 PatternID)
{
	auto Pattern = PatternMap[PatternID].GetDefaultObject();
	Pattern->PatternID = PatternID;

	SetPatternData(Pattern, PatternID);

	return Pattern->PatternData;
}

APattern* APatternManager::CreatePatternActor(int32 PatternID)
{
	auto Pattern = GetWorld()->SpawnActor<APattern>(PatternMap[PatternID]);

	Pattern->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SetPatternData(Pattern, PatternID);

	return Pattern;
}

void APatternManager::SetPatternData(APattern* Pattern, int32 PatternID)
{
	FPatternRawTable* PatternRawTableRow = GameManager->DataTableManager->GetPatternRawTableData(PatternID);
	
	Pattern->PatternData.PatternID = (*PatternRawTableRow).PatternID;
	Pattern->PatternData.PatternClass = (*PatternRawTableRow).PatternClass;
	Pattern->PatternData.Name = (*PatternRawTableRow).Name;
	Pattern->PatternData.Count = (*PatternRawTableRow).Count;
	Pattern->PatternData.AttackType = (*PatternRawTableRow).AttackType;
	Pattern->PatternData.DamagedType = (*PatternRawTableRow).DamagedType;
	Pattern->PatternData.SkillID = (*PatternRawTableRow).SkillID;
	Pattern->PatternData.CurHP = (*PatternRawTableRow).CurHP;
	Pattern->PatternData.PerHP = (*PatternRawTableRow).PerHP;
	Pattern->PatternData.CurST = (*PatternRawTableRow).CurST;
	Pattern->PatternData.PerST = (*PatternRawTableRow).PerST;

	Pattern->PatternData.PatternShape = PatternShape((*PatternRawTableRow).Shape, (*PatternRawTableRow).X, (*PatternRawTableRow).Y);

	Pattern->PatternData.UseParticle = (*PatternRawTableRow).UseParticle;
	Pattern->UseParticle->SetTemplate((*PatternRawTableRow).UseParticle);

	Pattern->PatternData.NotifyFieldParticle = (*PatternRawTableRow).NotifyFieldParticle;
	Pattern->NotifyFieldParticle->SetTemplate((*PatternRawTableRow).NotifyFieldParticle);

	Pattern->PatternData.DamageFieldParticle = (*PatternRawTableRow).DamageFieldParticle;
	Pattern->DamageFieldParticle->SetTemplate((*PatternRawTableRow).DamageFieldParticle);

	Pattern->PatternData.AnimationName = (*PatternRawTableRow).AnimationName;
	Pattern->AnimationName = (*PatternRawTableRow).AnimationName;
}

FPatternShape APatternManager::PatternShape(EShape Shape, float X, float Y)
{
	FPatternShape PatternShape;

	PatternShape.Shape = Shape;
	PatternShape.Size = FVector(X, Y, 0.f);

	return PatternShape;
}
