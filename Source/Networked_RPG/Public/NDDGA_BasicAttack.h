// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "NDDGameplayAbility.h"
#include "NDDGA_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API UNDDGA_BasicAttack : public UNDDGameplayAbility
{
	GENERATED_BODY()

protected:

	// Health Damage applied to the target
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damage")
	float DamageValue;

	// Health Damage applied to the target
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damage")
	float StunValue;

	// Gameplay Effect for applying damage to the target
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageGameplayEfect;
	
};
