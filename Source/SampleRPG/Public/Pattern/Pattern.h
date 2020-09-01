// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DataTableManager.h"

#include "Pattern.generated.h"

UCLASS()
class SAMPLERPG_API APattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APattern();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pattern|Properties")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pattern|Properties")
	class AGameManager* GameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pattern|Properties")
	class AMonster* Monster;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pattern|Properties")
	class AMainPlayer* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pattern|Properties")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class UBoxComponent* BoxCollision; 


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern|PatternTable")
	int32 PatternID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern|PatternTable")
	FPatternTable PatternData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class UParticleSystemComponent* UseParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class UParticleSystemComponent* NotifyFieldParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class UParticleSystemComponent* DamageFieldParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	FString AnimationName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Combat")
	FVector TargetLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Combat")
	FVector RandLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Combat")
	bool bIsNotifyFieldActive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void SetCollisionSize();
	void SetActiveCollision(bool bIsActive);

	void PlayUseParticle();
	void PlayNotifyParticle(bool bIsTargetLocation);
	void PlayDamageParticle(bool bIsTargetLocation);
	
	void SetTargetLocationByGroundSurface();
	void SetRandLocationByGroundSurface();
};
