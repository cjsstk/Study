// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StudyCharacter.h"
#include "HealthPointComponent.h"

AStudyProjectile::AStudyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	InitialLifeSpan = 2.0f;
}

void AStudyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->IgnoreActorWhenMoving(Instigator, true);
}

void AStudyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStudyProjectile::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other == this || Other == Instigator)
	{
		return;
	}

	AStudyCharacter* OtherCharacter = Cast<AStudyCharacter>(Other);
	if (OtherCharacter)
	{
		UHealthPointComponent* HealthComp = OtherCharacter->GetHealthPointComponent();
		if (ensure(HealthComp))
		{
			HealthComp->ApplyDamage(HitDamage);
		}
	}
	
	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, GetActorLocation(), FRotator::ZeroRotator, true);
	}

	Destroy();
}

