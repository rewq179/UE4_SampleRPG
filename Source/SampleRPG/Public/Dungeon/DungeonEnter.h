// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "DungeonEnter.generated.h"

UCLASS()
class SAMPLERPG_API ADungeonEnter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonEnter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DungeonEnter|ManagerClass")
	class AGameManager* GameManager;

	// Properties //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonEnter|Properties")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DungeonEnter|Properties")
	class USphereComponent* InteractCollision; // 영역에 들어왔는지 체크

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonEnter|Properties")
	class AMainPlayer* MainPlayer;

	// DataTable //
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonEnter|DataTable")
	int32 DungeonID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonEnter|DataTable")
	FDungeonTable DungeonData;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetDungeonData();

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
