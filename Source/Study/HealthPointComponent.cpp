// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPointComponent.h"

#include "StudyCharacter.h"

// Sets default values for this component's properties
UHealthPointComponent::UHealthPointComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealthPoint = MaxHealthPoint;
}

void UHealthPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AStudyCharacter * UHealthPointComponent::GetOwnerCharacter()
{
	return GetOwner() ? Cast<AStudyCharacter>(GetOwner()) : nullptr;
}

void UHealthPointComponent::SetHealthPoint(int32 InHealthPoint)
{
	const int32 NewHealthPoint = FMath::Clamp(InHealthPoint, 0, MaxHealthPoint);

	CurrentHealthPoint = NewHealthPoint;
}

void UHealthPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AStudyCharacter* Character = GetOwnerCharacter();
	if (Character)
	{
		Character->AddDebugString(FString::Printf(TEXT("HealthPoint: %d/%d"), CurrentHealthPoint, MaxHealthPoint));
	}
}

void UHealthPointComponent::ApplyDamage(int32 InDamage)
{
	const int32 NewHealthPoint = FMath::Clamp(CurrentHealthPoint - InDamage, 0, MaxHealthPoint);

	SetHealthPoint(NewHealthPoint);
}

