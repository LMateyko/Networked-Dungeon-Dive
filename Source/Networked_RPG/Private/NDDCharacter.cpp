// Copyright 2025 Lucas Mateyko


#include "NDDCharacter.h"
#include "Components/CapsuleComponent.h"

#include "../Networked_RPG.h"

#include "NDDAbilitySystemComponent.h"
#include "NDDAttributeSetBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ANDDCharacter::ANDDCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
}


//////////////////////////////////////////////////////////////////////////
// Attributes and Abilities

UAbilitySystemComponent* ANDDCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ANDDCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	if (IsValid(AbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(AbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent.Get(), this));
	}

	LogOnScreen(this, "Adding Character Abilities", FColor::Red, 10.0f);
	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ANDDCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
		return;

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	ApplyDefaultAttributesToEffectContext(EffectContext);
}

void ANDDCharacter::ApplyDefaultAttributesToEffectContext(FGameplayEffectContextHandle EffectContext)
{
	FGameplayEffectSpecHandle defaultAttributeHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (defaultAttributeHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*defaultAttributeHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

bool ANDDCharacter::IsAlive() const
{
	float health = GetHealth();
	float maxHealth = GetMaxHealth();

	UE_LOG(LogTemp, Display, TEXT("%s current health: %f/%f"), *GetName(), health, maxHealth);

	return true;
}

float ANDDCharacter::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float ANDDCharacter::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

