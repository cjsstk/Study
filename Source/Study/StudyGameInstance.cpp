// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyGameInstance.h"

static UDataTable* SkillDataTable = nullptr;

UStudyGameInstance::UStudyGameInstance()
{
	FString SkillDataPath = TEXT("/Game/Data/SkillData.SkillData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SkillData(*SkillDataPath);
	ensure(DT_SkillData.Succeeded());

	SkillDataTable = DT_SkillData.Object;
	check(SkillDataTable->RowMap.Num() > 0);

}

FSkillData * UStudyGameInstance::GetSkillData(int32 CmsSkillKey)
{
	if (!ensure(SkillDataTable))
	{
		return nullptr;
	}
	return SkillDataTable->FindRow<FSkillData>(*FString::FromInt(CmsSkillKey), TEXT(""));
}
