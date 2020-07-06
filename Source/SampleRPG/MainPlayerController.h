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
	// 에디터에서 유저 위젯을 등록할 의도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUD_OverlayAsset;

	// 위젯을 만든 후 위젯을 유지함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* HUD_Overlay;
	
protected:
	virtual void BeginPlay() override;

};
