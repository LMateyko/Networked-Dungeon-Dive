// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "NDDPlayerState.generated.h"

class UNDDAbilitySystemComponent;
class UNDDAttributeSetBase;

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

	class UNDDAttributeSetBase* GetAttributeSetBase() const;

	/**
	* Getters for attributes from UNDDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "ANDDPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ANDDPlayerState|Attributes")
	float GetMaxHealth() const;

protected:
	UPROPERTY()
	TObjectPtr<UNDDAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UNDDAttributeSetBase> AttributeSetBase;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;

	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
};
