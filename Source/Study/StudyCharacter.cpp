// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "StudyCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

#include "HealthPointComponent.h"
#include "SkillComponent.h"

//////////////////////////////////////////////////////////////////////////
// AStudyCharacter

static TAutoConsoleVariable<float> CVarTurnRate(
	TEXT("study.turnRate"),
	40.0f,
	TEXT("Camera turn rate"), ECVF_Cheat);

static TAutoConsoleVariable<float> CVarLookUpRate(
	TEXT("study.lookUpRate"),
	40.0f,
	TEXT("Camera look up rate"), ECVF_Cheat);

AStudyCharacter::AStudyCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

	HealthPointComponent = CreateDefaultSubobject<UHealthPointComponent>(TEXT("HealthPointComponent"));

}

void AStudyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AStudyCharacter::OnInputAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AStudyCharacter::OnReleaseAttack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStudyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStudyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStudyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStudyCharacter::LookUpAtRate);
}

void AStudyCharacter::OnInputAttack()
{
	if (ensure(SkillComponent))
	{
		FUseSkillParams Params;
		Params.CmsSkillKey = CurrentSkillNumber;
		SkillComponent->UseSkill(Params);
	}
}

void AStudyCharacter::OnReleaseAttack()
{
	if (ensure(SkillComponent))
	{
		SkillComponent->StopSkill();
		OnStopSkill.Broadcast();
	}
}

void AStudyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * CVarTurnRate.GetValueOnGameThread() * GetWorld()->GetDeltaSeconds());
}

void AStudyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * CVarLookUpRate.GetValueOnGameThread() * GetWorld()->GetDeltaSeconds());
}

void AStudyCharacter::Tick(float DeltaSeconds)
{

	if (!DebugString.IsEmpty())
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 100), DebugString, this, FColor::White, 0, true);

		DebugString.Empty();
	}
}

void AStudyCharacter::AddDebugString(const FString & InDebugString, bool bAddNewLine)
{
	DebugString += InDebugString;

	if (bAddNewLine)
	{
		DebugString += TEXT("\n");
	}
}

void AStudyCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	OnMoveInput.Broadcast(true, Value);
}

void AStudyCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}

	OnMoveInput.Broadcast(false, Value);
}
