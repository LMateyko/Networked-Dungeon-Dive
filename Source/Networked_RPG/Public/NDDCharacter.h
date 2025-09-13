// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Logging/LogMacros.h"

#include "NDDAbilitySet.h"

#include "NDDCharacter.generated.h"

class UGameplayEffect;

class UNDDAbilitySystemComponent;
class UNDDAttributeSetBase;

struct FGameplayAbilitySpecHandle;
struct FGameplayEffectContextHandle;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class NETWORKED_RPG_API ANDDCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ANDDCharacter();

	UFUNCTION(BlueprintCallable, Category = "ANDDCharacter")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "ANDDCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ANDDCharacter|Attributes")
	float GetMaxHealth() const;

protected:

	/* Variables */
	TWeakObjectPtr<UNDDAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UNDDAttributeSetBase> AttributeSetBase;

	bool AbilityBindingCompleted = false;


	/* Properties */
	// Binding of Ability to Inputs
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UNDDAbilitySet* AbilitySet;

	// Ability Handles granted by the ability set
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	/* Functions */

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	void InitializeAttributes();
	virtual void ApplyDefaultAttributesToEffectContext(FGameplayEffectContextHandle EffectContext);
};