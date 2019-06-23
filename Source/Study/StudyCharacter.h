// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SkillType.h"
#include "GameFramework/Character.h"
#include "StudyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterOnUseSkillAnimNotify);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterOnStopSkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterOnMoveInput, bool, bIsForward, float, Value);

USTRUCT(Blueprintable)
struct FAnimMontages
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShootProjectile;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ChannelingSkill;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ScopeSkill;
};

UCLASS(config=Game)
class AStudyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AStudyCharacter();

	virtual void Tick(float DeltaSeconds) override;

	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	class USkillComponent* GetSkillComponent() const { return SkillComponent; }
	class UHealthPointComponent* GetHealthPointComponent() const { return HealthPointComponent; }

	const FAnimMontages* GetCharacterMontages() const { return &CharacterMontages; }

	void AddDebugString(const FString& InDebugString, bool bAddNewLine = true);

	FCharacterOnUseSkillAnimNotify OnUseSkillAnimNotify;
	FCharacterOnStopSkill OnStopSkill;
	FCharacterOnMoveInput OnMoveInput;

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
	void OnReleaseAttack();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	class USkillComponent* SkillComponent;

	UPROPERTY(VisibleAnywhere)
	class UHealthPointComponent* HealthPointComponent;

	UPROPERTY(EditDefaultsOnly)
	FAnimMontages CharacterMontages;

	int32 CurrentSkillNumber = 3;

	FString DebugString;

};

