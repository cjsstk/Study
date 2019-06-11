// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SkillType.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "StudyGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSkillData() : Name(TEXT("")), SkillType(ESkillType::Invalid), ProjectileClass(FString(TEXT(""))), SpawnActorClass(FString(TEXT(""))) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSoftClassPtr<class AStudyProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSoftClassPtr<class AActor> SpawnActorClass;
};

/**
 * 
 */
UCLASS()
class STUDY_API UStudyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UStudyGameInstance();

	static FSkillData* GetSkillData(int32 CmsSkillKey);

private:

};

