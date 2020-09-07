// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DataTableManager.h"

#include "Pattern.generated.h"

/**
 * Pattern에서 사용될 정보를 모아놓은 클래스다.
*/

UCLASS()
class SAMPLERPG_API APattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APattern();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|ManagerClass")
	class AGameManager* GameManager;
	
	// Components //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Components")
	class AMonster* Monster;

	// Properties //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class UParticleSystemComponent* UseParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class UParticleSystemComponent* NotifyFieldParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Properties")
	class UParticleSystemComponent* DamageFieldParticle;

	// DataTable //
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern|DataTable")
	int32 PatternID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|DataTable")
	FPatternTable PatternData;

	// Combat //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Combat")
	class AMainPlayer* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Combat")
	FVector TargetLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Combat")
	FVector RandLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern|Combat")
	bool bIsRealtimeNotifyField;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override; // 실시간 Notify Field 용도로 사용중

	// Init & Setter //

	void InitCollision();
	void SetActiveCollision(bool bIsActive);

	// Monster의 MT와 연동(발동 시점)됨 //

	void PlayUseParticle();
	void PlayNotifyParticle(bool bIsTargetLocation);
	void PlayDamageParticle(bool bIsTargetLocation);
	
	// Get Surface Ground Location(바닥 찾기) //

	void SetTargetLocationByGroundSurface();
	void SetRandLocationByGroundSurface();

	// Overlap //

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
