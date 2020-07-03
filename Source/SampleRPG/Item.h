// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SAMPLERPG_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	int32 ItemId;

	class UDataTable* ItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemTable")
	FString Desc;

	/*
		Item���� �⺻���̽�

		�⺻ ��ƼƼ : �ݸ���(���� ����), �޽�(�������� ����), ��ƼŬ(��� ��ƼŬ, ���� ��ƼŬ), ����(���� ����), ȸ��(ȸ�� ����, ȸ�� �ӵ�)
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Collision")
	class USphereComponent* Collision; // ������ ���Դ��� üũ

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Mesh")
	class UStaticMeshComponent* Mesh; // �������� ���(����, �˵��� ���)

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particle")
	//class UParticleSystemComponent* IdleParticle; // ��� ��ƼŬ

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particle")
	//class UParticleSystem* InteractParticle; // ������ ������ ��ƼŬ

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	//class USoundCue* InteractSound; // ������ ����� ����

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Property")
	//bool bIsRotate; // �޽��� ȸ�� �Ұ��ΰ�?

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Property")
	//float RotateSpeed; // ȸ�� �ӵ�

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void RotateItem(float DeltaTime);

	//UFUNCTION() // ���� �ڽ� Ŭ�������� �������ϸ� UFUNCTION()�� �����ؾ��Ѵ�.
	//virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
