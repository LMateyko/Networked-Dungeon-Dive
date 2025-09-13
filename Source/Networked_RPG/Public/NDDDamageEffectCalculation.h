// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "NDDAttributeSetBase.h"

#include "NDDDamageEffectCalculation.generated.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct NDDDamageStatics
{
	//DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	NDDDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UNDDAttributeSetBase, Damage, Source, true);

		// Maintaining Armor example to help with future expansion using stats
		// Capture the Target's Armor. Don't snapshot.
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UNDDAttributeSetBase, Armor, Target, false);
	}
};

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API UNDDDamageEffectCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public: 
	UNDDDamageEffectCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
