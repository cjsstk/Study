// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyAnimNotify.h"

#include "Components/SkeletalMeshComponent.h"

#include "StudyCharacter.h"

void UStudyAnimNotify_Projectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AStudyCharacter* Character = MeshComp ? Cast<AStudyCharacter>(MeshComp->GetOwner()) : nullptr;
	if (Character)
	{
		ensure(SkillType != ESkillType::Invalid);

		Character->OnUseSkillAnimNotify.Broadcast(SkillType);
	}
}

