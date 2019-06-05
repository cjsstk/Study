// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthPointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STUDY_API UHealthPointComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthPointComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyDamage(int32 InDamage);

protected:
	virtual void BeginPlay() override;
	class AStudyCharacter* GetOwnerCharacter();

private:
	void SetHealthPoint(int32 InHealthPoint);

	UPROPERTY(EditDefaultsOnly)
	int32 MaxHealthPoint = 100;

	int32 CurrentHealthPoint = 100;
};
