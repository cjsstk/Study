// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SkillType.h"
#include "GameFramework/Character.h"
#include "StudyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterOnUseSkillAnimNotify, ESkillType, SkillType);

USTRUCT(Blueprintable)
struct FAnimMontages
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShootProjectile;

};

UCLASS(config=Game)
class AStudyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AStudyCharacter();

	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	class USkillComponent* GetSkillComponent() const { return SkillComponent; }

	const FAnimMontages* GetCharacterMontages() const { return &CharacterMontages; }

	FCharacterOnUseSkillAnimNotify OnUseSkillAnimNotify;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void OnInputAttack();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	class USkillComponent* SkillComponent;

	UPROPERTY(EditDefaultsOnly)
	FAnimMontages CharacterMontages;
};

