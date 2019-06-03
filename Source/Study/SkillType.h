#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Invalid,
	Projectile,
	Channeling,
	Count		UMETA(Hidden)
};