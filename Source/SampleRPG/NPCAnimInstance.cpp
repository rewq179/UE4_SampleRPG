// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCAnimInstance.h"
#include "NonPlayerCharacter.h"

void UNPCAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			NPC = Cast<ANonPlayerCharacter>(Pawn);
		}
	}
}

void UNPCAnimInstance::UpdateProperty()
{
	if (!Pawn || !NPC)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			NPC = Cast<ANonPlayerCharacter>(Pawn);
		}
	}

	if (NPC)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f); // X,Y 축의 속도만 알면 된다.

		MoveSpeed = LateralSpeed.Size();
	}
}





