// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Camera
	/*
		원리 : 셀카봉을 들고 뒤면 따라오듯이, 카메라를 스프링암에 고정시켜놓은다음 스프링암을 옮긴다.
	*/

	// 헤더파일 : "GameFramework/SpringArmComponent.h"
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bAbsoluteRotation = true; // 스프링 암의 회전이 루트 컴포넌트와 상위 컴포넌트를 따르지 않고 월드 좌표계의 회전을 따르도록 한다.
	CameraBoom->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	CameraBoom->TargetArmLength = 1000.f; // 셀카봉 길이는 어느정도로?
	CameraBoom->bDoCollisionTest = false; // 카메라가 벽을 뚫지 못하도록 만든다.

	//CameraBoom->bUsePawnControlRotation = true; // 카메라붐의 로테이션을 허용함

	// 헤더파일 : #include "Camera/CameraComponent.h"
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 카메라에따라 캐릭터를 움직이지 않겠다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// 헤더파일 : #include "GameFramework/CharacterMovementComponent.h"
	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터의 이동방향과 시야방향이 다르면 초당 640도의 회전 속도로 회전시킨다.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;
#pragma endregion

#pragma region InputKeyBool
	bIsSprint = false;

#pragma endregion

#pragma region Status
	NormalSpeed = 600.f;
	SprintSpeed = 1000.f;
#pragma endregion



}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // PlayerInputComponent 유무 체크

	// 헤더파일 : #include "GameFramework/PlayerController.h"

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::LeftClickDown);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("RightLeftView", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("UpDownView", this, &APawn::AddControllerPitchInput);
}

#pragma region Sprint
void AMainPlayer::SetMovementState(EMovementState State) // 캐릭터상태에따라 최대속도가 변화됨
{
	MovementState = State;

	// 헤더파일 : #include "GameFramework/PlayerController.h"

	if (MovementState == EMovementState::EMS_Sprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // 최대 속도를 스프린트 속도로
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // 최대 속도를 일반 달리기 속도로
	}
}

void AMainPlayer::LeftShiftKeyDown()
{
	SetMovementState(EMovementState::EMS_Sprint);

	bIsSprint = true;
}

void AMainPlayer::LeftShiftKeyUp()
{
	SetMovementState(EMovementState::EMS_Idle);

	bIsSprint = false;
}


#pragma endregion



void AMainPlayer::MoveForward(float Value)
{
	if (Value != 0.f && Controller)
	{
		FRotator Rotation = Controller->GetControlRotation(); // 현재 캐릭터의 회전 매트릭스를 구한다.
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// FRotationMatrix : (0,0,0)과 비교해 각 축들이 얼마나 회전 되어있는지를 계산한다.
		// GetUnitAxis : GetSacledAxis의 결과를 단위 벡터(길이를 1로 변환)하여 돌려주는 함수다.

		AddMovementInput(Direction, Value);

		/*
			다른 방법 :

			FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		*/
	}
}

void AMainPlayer::MoveRight(float Value)
{
	if (Value != 0.f && Controller)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::LeftClickDown()
{

}
