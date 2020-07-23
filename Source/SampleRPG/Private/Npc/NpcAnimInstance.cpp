// Fill out your copyright notice in the Description page of Project Settings.

#include "Npc/NpcAnimInstance.h"
#include "Npc/NpcController.h"

void UNpcAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Npc = Cast<ANpcController>(Pawn);
		}
	}
}

void UNpcAnimInstance::UpdateProperty()
{
	if (!Pawn || !Npc)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Npc = Cast<ANpcController>(Pawn);
		}
	}

	if (Npc)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f); // X,Y 축의 속도만 알면 된다.

		MoveSpeed = LateralSpeed.Size();
	}
}





