// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

#include "AsyncTaskAbilityModeChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityModeChanged);

/**
 * Blueprint node to automatically register listener for changes in the player's Ability Mode tags
 * Used for adjusting UI to reflect current state
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class NETWORKED_RPG_API UAsyncTaskAbilityModeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilityModeChanged OnAbilityModeChanged;

	// Listens for changes to AbilityMode tags to adjust UI based on the current state
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncTaskAbilityModeChanged* ListenForAbilityModeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer ModeTag);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTagContainer TrackedModeTags;

	virtual void AbilityModeChanged(const FGameplayTag abilityModeTag, int32 newCount);

};
