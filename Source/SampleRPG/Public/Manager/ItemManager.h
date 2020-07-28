// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Item.h"

#include "ItemManager.generated.h"

UCLASS()
class SAMPLERPG_API AItemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Data")
	TMap<int32, TSubclassOf<class AItem>> ItemMap;

	UFUNCTION(BlueprintCallable)
	FItemTable GetItemTableValue(int32 ItemID);

	UFUNCTION(BlueprintCallable)
	class AItem* CreateItemActor(int32 ItemID, int32 Count);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame


	
};
