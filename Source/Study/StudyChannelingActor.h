// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "StudyChannelingActor.generated.h"

UCLASS()
class STUDY_API AStudyChannelingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AStudyChannelingActor();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnStopSkill();

	void TickConsiderSelectTarget();

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystemComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY()
	AActor* TargetActor = nullptr;

	UPROPERTY()
	TArray<AActor*> TargetActors;

	UPROPERTY(EditDefaultsOnly)
	float ChangeTargetTimeSeconds = 1.0f;

	float ChangeTargetTimeAgeSeconds = 0.0f;

};
