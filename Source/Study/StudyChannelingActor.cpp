// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyChannelingActor.h"

#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"

#include "StudyCharacter.h"

const static FName SourceParamName = FName(TEXT("BeamSource"));
const static FName TargetParamName = FName(TEXT("BeamTarget"));

AStudyChannelingActor::AStudyChannelingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(ParticleSystemComponent);

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AStudyChannelingActor::OnBeginOverlapComponent);
	BoxComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AStudyChannelingActor::OnEndOverlapComponent);
}

void AStudyChannelingActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensure(Instigator))
	{
		AStudyCharacter* Character = Cast<AStudyCharacter>(Instigator);
		if (ensure(Character))
		{
			Character->OnStopSkill.AddUniqueDynamic(this, &AStudyChannelingActor::OnStopSkill);
			ParticleSystemComponent->SetActorParameter(SourceParamName, this);
			ParticleSystemComponent->SetActorParameter(TargetParamName, nullptr);
		}
	}

	ChangeTargetTimeAgeSeconds = ChangeTargetTimeSeconds;
}

void AStudyChannelingActor::OnBeginOverlapComponent(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	TargetActors.AddUnique(OtherActor);
}

void AStudyChannelingActor::OnEndOverlapComponent(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	TargetActors.Remove(OtherActor);
}

void AStudyChannelingActor::OnStopSkill()
{
	Destroy();
}

void AStudyChannelingActor::TickConsiderSelectTarget()
{
	float NearestDistance = MAX_FLT;
	AActor* NearestActor = nullptr;
	for (AActor* Actor : TargetActors)
	{
		const FVector SourceLocation = Instigator->GetActorLocation();
		const FVector TargetLocation = Actor->GetActorLocation();

		const float TargetDistance = FVector::DistSquared(SourceLocation, TargetLocation);
		if (TargetDistance < NearestDistance)
		{
			NearestDistance = TargetDistance;
			NearestActor = Actor;
		}
	}

	TargetActor = NearestActor;
}

void AStudyChannelingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeTargetTimeAgeSeconds -= DeltaTime;
	if (ChangeTargetTimeAgeSeconds < 0)
	{
		ChangeTargetTimeAgeSeconds = ChangeTargetTimeSeconds;
		TickConsiderSelectTarget();

		ParticleSystemComponent->SetActorParameter(TargetParamName, TargetActor ? TargetActor : nullptr);
	}

	if (Instigator)
	{
		AStudyCharacter* Character = Cast<AStudyCharacter>(Instigator);
		if (ensure(Character))
		{
			const FRotator CharacterControllerRoatation = Character->GetControlRotation();
			SetActorRotation(CharacterControllerRoatation);
		}
	}

}

