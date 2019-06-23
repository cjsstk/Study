// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillComponent.h"

#include "Animation/AnimInstance.h"

#include "StudyCharacter.h"
#include "StudyProjectile.h"
#include "StudyScopeSkillActor.h"
#include "StudyGameInstance.h"

const static FName SectionEnd = FName(TEXT("End"));

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

void USkillComponent::StopAnimMontage()
{
	if (!bPlaying)
	{
		ensure(0);
		return;
	}

	if (!ensure(AnimMontage))
	{
		return;
	}

	AStudyCharacter* Character = GetCharacter();
	if (!ensure(Character))
	{
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;

	if (!ensure(AnimInstance))
	{
		return;
	}

	AnimInstance->Montage_Stop(0.1f, AnimMontage);

	bPlaying = false;
}

void USkillComponent::JumpToSectionAnimMontage(const FName SectionName)
{
	if (!bPlaying)
	{
		ensure(0);
		return;
	}

	if (!ensure(AnimMontage))
	{
		return;
	}

	AStudyCharacter* Character = GetCharacter();
	if (!ensure(Character))
	{
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;

	if (!ensure(AnimInstance))
	{
		return;
	}

	AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
}

void USkillComponent::FinishSkill()
{
	AStudyCharacter* Character = GetCharacter();
	if (ensure(Character))
	{
		Character->OnUseSkillAnimNotify.RemoveDynamic(this, &USkillComponent::OnUseSkillAnimNotify);
		Character->GetController()->SetIgnoreMoveInput(false);
	}
	
	SpawnActor = nullptr;
	bPlaying = false;
	CurrentSkillCmsKey = 0;
	CurrentSkillType = ESkillType::Invalid;
}

void USkillComponent::OnUseSkillAnimNotify()
{
	switch (CurrentSkillType)
	{
	case ESkillType::Projectile:
	{
		AStudyCharacter* Character = Cast<AStudyCharacter>(GetOwner());
		if (ensure(Character))
		{
			const FSkillData* SkillData = UStudyGameInstance::GetSkillData(CurrentSkillCmsKey);
			if (!ensure(SkillData))
			{
				return;
			}

			UClass* ProjectileClass = SkillData->ProjectileClass.LoadSynchronous();
			if (ensure(ProjectileClass))
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Instigator = Character;

				const FRotator ShootDirection = Character->GetControlRotation();

				const static FName SocketName = FName(TEXT("SpawnLocation"));
				const FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(SocketName);

				AStudyProjectile* Projectile = GetWorld()->SpawnActor<AStudyProjectile>(ProjectileClass, SpawnLocation, ShootDirection, SpawnParams);
				if (!ensure(Projectile))
				{
					return;
				}
			}
		}
		break;
	}
	case ESkillType::Channeling:
		break;
	case ESkillType::Scope:
	{
		AStudyScopeSkillActor* ScopeActor = Cast<AStudyScopeSkillActor>(SpawnActor);
		if (ensure(ScopeActor))
		{
			ScopeActor->ExplosionScope();
		}
		break;
	}
	default:
		break;
	}
}

void USkillComponent::OnMontageEnded(UAnimMontage * EndedAnimMontage, bool bInterrupted)
{
	FinishSkill();
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

	const FSkillData* SkillData = UStudyGameInstance::GetSkillData(UseSkillParams.CmsSkillKey);
	if (!ensure(SkillData))
	{
		return;
	}

	CurrentSkillCmsKey = UseSkillParams.CmsSkillKey;
	CurrentSkillType = SkillData->SkillType;

	UClass* SpawnActorClass = SkillData->SpawnActorClass.LoadSynchronous();
	if (ensure(SpawnActorClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Character;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const static FName SocketName = FName(TEXT("SpawnLocation"));
		const FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(SocketName) + (Character->GetActorForwardVector() * 50.0f) + (Character->GetActorUpVector() * 50.0f);

		SpawnActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, SpawnLocation, Character->GetActorRotation(), SpawnParams);
		if (ensure(SpawnActor) && CurrentSkillType != ESkillType::Scope)
		{
			SpawnActor->AttachToActor(Character, FAttachmentTransformRules::KeepWorldTransform, SocketName);
		}
	}

	switch (CurrentSkillType)
	{
	case ESkillType::Projectile:
		SetAnimMontage(Character->GetCharacterMontages()->ShootProjectile);
		break;
	case ESkillType::Channeling:
		SetAnimMontage(Character->GetCharacterMontages()->ChannelingSkill);
		break;
	case ESkillType::Scope:
		Character->GetController()->SetIgnoreMoveInput(true);
		SetAnimMontage(Character->GetCharacterMontages()->ScopeSkill);
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

void USkillComponent::StopSkill()
{
	switch (CurrentSkillType)
	{
	case ESkillType::Projectile:
		return;
	case ESkillType::Channeling:
		if (bPlaying)
		{
			StopAnimMontage();
		}
		FinishSkill();
		break;
	case ESkillType::Scope:
		JumpToSectionAnimMontage(SectionEnd);
		break;
	default:
		break;
	}
}

