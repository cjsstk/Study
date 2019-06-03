// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	
}

void AStudyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

