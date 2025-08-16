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
		//handles.AddUnique(AbilitySystemComponent->GiveAbility(
		//	FGameplayAbilitySpec(
		//		abilitySetItem.GameplayAbility,
		//		0,
		//		static_cast<uint32>(abilitySetItem.InputKey)
		//	)));
		//handles.AddUnique(AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(abilitySetItem.GameplayAbility)));

		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(
				abilitySetItem.GameplayAbility,
				1, 
				static_cast<int32>(abilitySetItem.GameplayAbility.GetDefaultObject()->AbilityInputID),
				InSourceObject));
	}

	return handles;
}
