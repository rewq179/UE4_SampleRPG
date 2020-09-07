// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NormalAttack.generated.h"

/**
 * Pawn�� NormalAttack �ִϸ��̼��� ���/������ ����/�����̻� ������ ���������� ó���Ѵ�.
 */
UCLASS()
class SAMPLERPG_API UBTTask_NormalAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_NormalAttack();
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(Category = "Value", EditAnywhere)
	int32 AttackNumber; // Montage�� ����Ǿ��ִ� Attack_?���� ?�� �� ���ڸ� �ǹ��Ѵ�.

	bool IsAttacking; // ���� ó���� �������̸� True, ������ False
};
