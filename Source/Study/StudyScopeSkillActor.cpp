// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyScopeSkillActor.h"

#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "DrawDebugHelpers.h"

#include "StudyCharacter.h"
#include "HealthPointComponent.h"

static TAutoConsoleVariable<int32> CVarScopeParticleDebug(
	TEXT("study.scopeParticleDebug"),
	0,
	TEXT("1: enable debug, 0: disable debug"), ECVF_Cheat);

// Sets default values
AStudyScopeSkillActor::AStudyScopeSkillActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AStudyScopeSkillActor::OnBeginOverlapComponent);
	BoxComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AStudyScopeSkillActor::OnEndOverlapComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(BoxComponent);

}

void AStudyScopeSkillActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Instigator)
	{
		AStudyCharacter* Character = Cast<AStudyCharacter>(Instigator);
		if (ensure(Character))
		{
			Character->OnMoveInput.AddUniqueDynamic(this, &AStudyScopeSkillActor::MoveActor);
		}
	}
}

void AStudyScopeSkillActor::OnBeginOverlapComponent(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this || OtherActor == Instigator)
	{
		return;
	}

	TargetActors.AddUnique(OtherActor);
}

void AStudyScopeSkillActor::OnEndOverlapComponent(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	TargetActors.Remove(OtherActor);
}

void AStudyScopeSkillActor::MoveActor(bool bIsForward, float Value)
{
	FVector MoveDirection;

	if (bIsForward)
	{
		MoveDirection = GetActorForwardVector();
	}
	else
	{
		MoveDirection = GetActorRightVector();
	}

	SetActorLocation(GetActorLocation() + (MoveDirection * Value * ActorMoveSpeed));
}

void AStudyScopeSkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStudyScopeSkillActor::ExplosionScope()
{
	FHitResult Hit;

	const FVector StartLocation = GetActorLocation() + (GetActorUpVector() * 500.0f);
	const FVector EndLocation = GetActorLocation() + (GetActorUpVector() * -500.0f);

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldStatic);

	if (CVarScopeParticleDebug.GetValueOnGameThread() > 0)
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 3.0f, 0, 10);
	}

	if (ParticleSystem)
	{
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, Hit.Location, FRotator::ZeroRotator, true);
	}

	for (AActor* TargetActor : TargetActors)
	{
		UHealthPointComponent* HealthComp = TargetActor->FindComponentByClass<UHealthPointComponent>();
		if (HealthComp)
		{
			HealthComp->ApplyDamage(Damage);
		}
	}

	Destroy();
}

