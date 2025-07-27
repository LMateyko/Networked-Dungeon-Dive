// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "NDDPlayerState.generated.h"

class UNDDAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API ANDDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public: 
	ANDDPlayerState();

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<UNDDAbilitySystemComponent> AbilitySystemComponent;

};
