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
	
	static const FName HomeLocation; // Ű �̸��� �ٲ��� �ʾ� ���� �ʱⰪ�� �����Ѵ�. ������ �������� �ϵ��ڵ����� ���� �����ؾ��Ѵ�.
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
	virtual void Possess(APawn* InPawn) override; // 4.22���ʹ� OnPosses��. ������ Possess�� ����ؾ��Ѵ�. �ƴϸ� GetPawn()���� Pawn�� ���� �� ����.

	void StopAI();
	void StartAI();
};
