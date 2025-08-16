// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../Networked_RPG.h"

#include "NDDGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API UNDDGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UNDDGameplayAbility();

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	ENDDAbilityInputID AbilityInputID = ENDDAbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	ENDDAbilityInputID AbilityID = ENDDAbilityInputID::None;
};
