// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DataTableManager.h"

#include "Skill.generated.h"

UCLASS()
class SAMPLERPG_API ASkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkill();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill|Properties")
	class AMonster* Monster;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill|Properties")
	class AMainPlayer* MainPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill|Properties")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Properties")
	class UBoxComponent* BoxCollision; 


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|SkillTable")
	int32 SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|SkillTable")
	FSkillTable SkillData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Properties")
	UParticleSystemComponent* UseParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Properties")
	UParticleSystemComponent* NotifyFieldParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Properties")
	UParticleSystemComponent* DamageFieldParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Properties")
	FString AnimationName;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void SetCollisionSize(FSkillShape SkillShape);
};
