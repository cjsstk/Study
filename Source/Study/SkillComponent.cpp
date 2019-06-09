// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillComponent.h"

#include "Animation/AnimInstance.h"

#include "StudyCharacter.h"
#include "StudyProjectile.h"
#include "StudyGameInstance.h"

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

void USkillComponent::OnUseSkillAnimNotify()
{
	switch (CurrentSkillType)
	{
	case ESkillType::Projectile:
	{
		AStudyCharacter* Character = Cast<AStudyCharacter>(GetOwner());
		if (ensure(Character))
		{
			if (!ProjectileClassPath.IsEmpty())
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Instigator = Character;

				const FRotator ShootDirection = Character->GetControlRotation();

				const static FName SocketName = FName(TEXT("SpawnLocation"));
				const FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(SocketName);

				const FSoftClassPath ProjectilePath = ProjectileClassPath;
				UClass* ProjectileClass = ProjectilePath.TryLoadClass<AStudyProjectile>();

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
	ProjectileClassPath.Empty();
	CurrentSkillType = ESkillType::Invalid;
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

	CurrentSkillType = SkillData->SkillType;
	ProjectileClassPath = SkillData->ProjectileClass;
	if (!SkillData->SpawnActorClass.IsEmpty())
	{
		const FSoftClassPath SpawnActorPath = SkillData->SpawnActorClass;
		UClass* SpawnActorClass = SpawnActorPath.TryLoadClass<AActor>();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Character;

		AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, Character->GetActorLocation(), Character->GetActorRotation(), SpawnParams);
	}

	switch (CurrentSkillType)
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

