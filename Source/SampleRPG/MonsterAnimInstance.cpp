// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAnimInstance.h"
#include "Monster.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Monster = Cast<AMonster>(Pawn);
		}
	}
}

void UMonsterAnimInstance::UpdateProperty()
{
	if (!Pawn || !Monster)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Monster = Cast<AMonster>(Pawn);
		}
	}

	if (Monster)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f); // X,Y 축의 속도만 알면 된다.
		
		MoveSpeed = LateralSpeed.Size();
	}
}


