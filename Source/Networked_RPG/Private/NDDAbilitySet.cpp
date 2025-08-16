// Fill out your copyright notice in the Description page of Project Settings.


#include "NDDAbilitySet.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"

TArray<FGameplayAbilitySpecHandle> UNDDAbilitySet::GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent, UObject* InSourceObject) const
{
	check(AbilitySystemComponent);

	TArray<FGameplayAbilitySpecHandle> handles;

	for (const auto abilitySetItem : AbilitySetItems)
	{
		// Consider that FGameplayAbilitySpec can take level, input key, and SourceObject as well
		handles.AddUnique(AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(abilitySetItem.GameplayAbility)));
	}

	return handles;
}
