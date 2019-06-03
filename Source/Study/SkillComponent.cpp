// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillComponent.h"

#include "Animation/AnimInstance.h"

#include "StudyCharacter.h"
#include "StudyProjectile.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AStudyCharacter * USkillComponent::GetCharacter()
{
	return Cast<AStudyCharacter>(GetOwner());
}

void USkillComponent::SetAnimMontage(class UAnimMontage* NewAnimMontage)
{
	if (bPlaying)
	{
		ensureMsgf(0, TEXT("You can only change montage before play"));
		return;
	}

	AnimMontage = NewAnimMontage;
}

bool USkillComponent::PlayAnimMontage()
{
	if (bPlaying)
	{
		ensure(0);
		return false;
	}

	if (!ensure(AnimMontage))
	{
		return false;
	}

	AStudyCharacter* Character = GetCharacter();
	if (!ensure(Character))
	{
		return false;
	}

	UAnimInstance* AnimInstance = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;

	if (!ensure(AnimInstance))
	{
		return false;
	}

	const float Duration = AnimInstance->Montage_Play(AnimMontage, 1.0f);

	if (Duration == 0)
	{
		return false;
	}

	bPlaying = true;

	Character->OnUseSkillAnimNotify.AddUniqueDynamic(this, &USkillComponent::OnUseSkillAnimNotify);
	AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &USkillComponent::OnMontageEnded);

	return true;
}

void USkillComponent::OnUseSkillAnimNotify(ESkillType SkillType)
{
	switch (SkillType)
	{
	case ESkillType::Projectile:
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		AStudyProjectile* Projectile = GetWorld()->SpawnActor<AStudyProjectile>(ProjectileClass.Get(), GetOwner()->GetActorLocation(), GetOwner()->GetActorForwardVector().Rotation(), SpawnParams);
		break;
	}
	case ESkillType::Channeling:
		break;
	default:
		break;
	}
}

void USkillComponent::OnMontageEnded(UAnimMontage * EndedAnimMontage, bool bInterrupted)
{
	AStudyCharacter* Character = GetCharacter();
	if (ensure(Character))
	{
		Character->OnUseSkillAnimNotify.RemoveDynamic(this, &USkillComponent::OnUseSkillAnimNotify);
	}

	bPlaying = false;
}

bool USkillComponent::CanUseSkill()
{
	if (bPlaying)
	{
		return false;
	}

	return true;
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillComponent::UseSkill(FUseSkillParams UseSkillParams)
{
	if (!CanUseSkill())
	{
		return;
	}

	AStudyCharacter* Character = GetCharacter();
	if (!ensure(Character))
	{
		return;
	}

	switch (UseSkillParams.SkillType)
	{
	case ESkillType::Projectile:
		SetAnimMontage(Character->GetCharacterMontages()->ShootProjectile);
		break;
	default:
		ensure(0);
		break;
	}

	const bool bPlayed = PlayAnimMontage();
	if (!bPlayed)
	{
		ensure(0);
		return;
	}
}

