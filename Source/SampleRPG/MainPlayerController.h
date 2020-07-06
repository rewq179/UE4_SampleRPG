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
	// �����Ϳ��� ���� ������ ����� �ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUD_OverlayAsset;

	// ������ ���� �� ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* HUD_Overlay;
	
protected:
	virtual void BeginPlay() override;

};
