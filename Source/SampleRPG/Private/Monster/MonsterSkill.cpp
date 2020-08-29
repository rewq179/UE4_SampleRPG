// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterSkill.h"
#include "Monster.h"
#include "Skill.h"
#include "GameManager.h"

// Sets default values
AMonsterSkill::AMonsterSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterSkill::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMonsterSkill::AddSkill(int32 SkillID)
{
	auto Skill = GameManager->SkillManager->CreateSkillActor(SkillID);
	Skill->Monster = Monster;

	Skill->SetCollisionSize(Skill->SkillData.SkillShape);

	SkillMaps.Add(SkillID, Skill);
}

void AMonsterSkill::ApplySkillDamage(int32 SkillID)
{


}

