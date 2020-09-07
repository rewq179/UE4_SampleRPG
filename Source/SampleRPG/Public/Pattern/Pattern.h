// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DataTableManager.h"

#include "Pattern.generated.h"

/**
 * Pattern���� ���� ������ ��Ƴ��� Ŭ������.
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
	virtual void Tick(float DeltaTime) override; // �ǽð� Notify Field �뵵�� �����

	// Init & Setter //

	void InitCollision();
	void SetActiveCollision(bool bIsActive);

	// Monster�� MT�� ����(�ߵ� ����)�� //

	void PlayUseParticle();
	void PlayNotifyParticle(bool bIsTargetLocation);
	void PlayDamageParticle(bool bIsTargetLocation);
	
	// Get Surface Ground Location(�ٴ� ã��) //

	void SetTargetLocationByGroundSurface();
	void SetRandLocationByGroundSurface();

	// Overlap //

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
