// Fill out your copyright notice in the Description page of Project Settings.


#include "NDDGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

UNDDGameplayAbility::UNDDGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
