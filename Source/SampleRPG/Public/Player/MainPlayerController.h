// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLERPG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD|Properties")
	TSubclassOf<class UUserWidget> HUD_OverlayAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD|Properties")
	UUserWidget* HUD_Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Properties")
	class AMainPlayer* MainPlayer;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|Player")
	UUserWidget* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|Player")
	UUserWidget* Equipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|Player")
	UUserWidget* PlayerHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|Monster")
	UUserWidget* MonsterHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|System")
	UUserWidget* PauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|System")
	UUserWidget* ToolTip;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|System")
	UUserWidget* ItemStore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Widgets|System")
	UUserWidget* Dialogue;*/


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
