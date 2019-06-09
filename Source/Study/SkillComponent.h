// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillType.h"
#include "SkillComponent.generated.h"


USTRUCT(Blueprintable)
struct FUseSkillParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 CmsSkillKey = 0;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STUDY_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UseSkill(FUseSkillParams UseSkillParams);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class AStudyCharacter* GetCharacter();
	void SetAnimMontage(class UAnimMontage* NewAnimMontage);
	bool PlayAnimMontage();

	UFUNCTION()
	void OnUseSkillAnimNotify();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* EndedAnimMontage, bool bInterrupted);

	bool CanUseSkill();

	UPROPERTY()
	class UAnimMontage* AnimMontage = nullptr;

	bool bPlaying = false;
	
	/** 
	 * SkillData
	 */
	ESkillType CurrentSkillType = ESkillType::Invalid;

	FString ProjectileClassPath;

};
