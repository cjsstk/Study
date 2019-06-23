// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "StudyScopeSkillActor.generated.h"

UCLASS()
class STUDY_API AStudyScopeSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AStudyScopeSkillActor();

	virtual void Tick(float DeltaTime) override;

	void ExplosionScope();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void MoveActor(bool bIsForward, float Value);

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UDecalComponent* DecalComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ParticleSystem;

	UPROPERTY()
	TArray<AActor*> TargetActors;

	UPROPERTY(EditDefaultsOnly)
	int32 Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float ActorMoveSpeed = 10.0f;

};
