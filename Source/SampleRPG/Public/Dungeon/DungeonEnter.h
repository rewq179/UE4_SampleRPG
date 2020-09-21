// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Manager/DataTableManager.h"

#include "DungeonEnter.generated.h"

/**
* 마을에서 던전으로 이동시켜주는 오브젝트다.
*/

UCLASS()
class SAMPLERPG_API ADungeonEnter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonEnter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DungeonEnter|ManagerClass")
	class AGameManager* GameManager;

	/* Properties */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonEnter|Properties")
	class AMainPlayer* MainPlayer;

	// Component //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonEnter|Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DungeonEnter|Components")
	class USphereComponent* InteractCollision; // 영역에 들어왔는지 체크

	// DataTable //
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonEnter|DataTable")
	TArray<int32> DungeonIDs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DungeonEnter|DataTable")
	TArray<FDungeonTable> DungeonDatas;


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
