// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainPlayer.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			MainPlayer = Cast<AMainPlayer>(Pawn);
		}
	}
}

void UPlayerAnimInstance::UpdateProperty()
{
	if (Pawn == nullptr || MainPlayer == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			MainPlayer = Cast<AMainPlayer>(Pawn);
		}
	}

	if (MainPlayer)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f); // X,Y ���� �ӵ��� �˸� �ȴ�.
		MoveSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
}


