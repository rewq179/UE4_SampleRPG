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
		Item들의 기본베이스

		기본 엔티티 : 콜리젼(반응 영역), 메쉬(아이템의 형태), 파티클(평소 파티클, 반응 파티클), 사운드(반응 사운드), 회전(회젼 여부, 회전 속도)
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Collision")
	class USphereComponent* Collision; // 영역에 들어왔는지 체크

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Mesh")
	class UStaticMeshComponent* Mesh; // 아이템의 모습(코인, 검등의 모습)

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particle")
	//class UParticleSystemComponent* IdleParticle; // 평소 파티클

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particle")
	//class UParticleSystem* InteractParticle; // 반응시 보여줄 파티클

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	//class USoundCue* InteractSound; // 반응시 들려줄 사운드

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Property")
	//bool bIsRotate; // 메쉬가 회전 할것인가?

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Property")
	//float RotateSpeed; // 회전 속도

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void RotateItem(float DeltaTime);

	//UFUNCTION() // 만약 자식 클래스에서 재정의하면 UFUNCTION()을 제거해야한다.
	//virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
