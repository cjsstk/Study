// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillType.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StudyAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class STUDY_API UStudyAnimNotify_Projectile : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
private:
	UPROPERTY(EditAnywhere)
	ESkillType SkillType = ESkillType::Invalid;
};
