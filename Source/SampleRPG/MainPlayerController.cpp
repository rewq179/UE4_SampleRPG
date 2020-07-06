// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();


	// 헤더파일 : #include "Blueprint/UserWidget.h"

	if (HUD_OverlayAsset)
	{
		HUD_Overlay = CreateWidget<UUserWidget>(this, HUD_OverlayAsset);
	}

	HUD_Overlay->AddToViewport();
	HUD_Overlay->SetVisibility(ESlateVisibility::Visible);

}




