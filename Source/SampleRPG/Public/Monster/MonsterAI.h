// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAI.generated.h"

/**
 * 
 */

UCLASS()
class SAMPLERPG_API AMonsterAI : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAI();
	
	static const FName HomeLocation; // 키 이름이 바뀌지 않아 변수 초기값을 지정한다. 참조는 편하지만 하드코딩으로 값을 변경해야한다.
	static const FName PatrolLocation;
	static const FName Target;
	static const FName InDistance;
	static const FName AttackPattern;

private:
	UPROPERTY(EditAnywhere, Category = "Monster|BehaviorTree")
	class UBlackboardData* BBAsset;

	UPROPERTY(EditAnywhere, Category = "Monster|BehaviorTree")
	class UBehaviorTree* BTAsset;

public:
	virtual void Possess(APawn* InPawn) override; // 4.22부터는 OnPosses다. 지금은 Possess를 사용해야한다. 아니면 GetPawn()으로 Pawn을 얻을 수 없음.

	void StopAI();
	void StartAI();
};
