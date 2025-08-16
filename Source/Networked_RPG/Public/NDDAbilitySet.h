// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Networked_RPG.h"
#include "NDDGameplayAbility.h"
#include "NDDAbilitySet.generated.h"

class UGameplayAbility;
class NDDGameplayAbility;
class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;

// Struct to contain data for tie Abilities to an Enhanced Input 
USTRUCT()
struct FNDDAbilitySetItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UNDDGameplayAbility> GameplayAbility;
};

UCLASS()
class NETWORKED_RPG_API UNDDAbilitySet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<FNDDAbilitySetItem> AbilitySetItems;

	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent, UObject* InSourceObject) const;
};
