// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Networked_RPG.h"
#include "NDDAbilitySet.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;

// Struct to contain data for tie Abilities to an Enhanced Input 
USTRUCT()
struct FNDDAbilitySetItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	ENDDAbilityInput InputKey;
};

UCLASS()
class NETWORKED_RPG_API UNDDAbilitySet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<FNDDAbilitySetItem> AbilitySetItems;

	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;
};
