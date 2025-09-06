// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTaskAbilityModeChanged.h"

UAsyncTaskAbilityModeChanged* UAsyncTaskAbilityModeChanged::ListenForAbilityModeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer ModeTags)
{
	UAsyncTaskAbilityModeChanged* ListenForAbilityModeChange = NewObject<UAsyncTaskAbilityModeChanged>();
	ListenForAbilityModeChange->ASC = AbilitySystemComponent;
	ListenForAbilityModeChange->TrackedModeTags = ModeTags;

	// Check to see if the ability system and desired tag are valid. 
	// If no, exit early. 
	if (!IsValid(AbilitySystemComponent) || ModeTags.Num() < 1)
	{
		ListenForAbilityModeChange->EndTask();
		return nullptr;
	}

	TArray<FGameplayTag> ModeTagArray;
	ModeTags.GetGameplayTagArray(ModeTagArray);

	for (FGameplayTag ModeTag : ModeTagArray)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(ModeTag, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(ListenForAbilityModeChange, &UAsyncTaskAbilityModeChanged::AbilityModeChanged);
	}

	return ListenForAbilityModeChange;
}

void UAsyncTaskAbilityModeChanged::EndTask()
{
	if (IsValid(ASC))
	{
		TArray<FGameplayTag> ModeTagArray;
		TrackedModeTags.GetGameplayTagArray(ModeTagArray);

		for (FGameplayTag ModeTag : ModeTagArray)
		{
			ASC->RegisterGameplayTagEvent(ModeTag, EGameplayTagEventType::NewOrRemoved)
				.RemoveAll(this);
		}
	}
}

void UAsyncTaskAbilityModeChanged::AbilityModeChanged(const FGameplayTag abilityModeTag, int32 newCount)
{
	if (newCount > 0)
	{
		OnAbilityModeChanged.Broadcast();
	}
}
