// Copyright 2025 Lucas Mateyko


#include "NDDPlayerState.h"
#include "NDDAbilitySystemComponent.h"
#include "NDDAttributeSetBase.h"

ANDDPlayerState::ANDDPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UNDDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSetBase = CreateDefaultSubobject<UNDDAttributeSetBase>(TEXT("AttributeSetBase"));
}

UAbilitySystemComponent* ANDDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UNDDAttributeSetBase* ANDDPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

//////////////////////////////////////////////////////////////////////////
// Attribute Getters

float ANDDPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ANDDPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

// Called when the game starts or when spawned
void ANDDPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ANDDPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &ANDDPlayerState::MaxHealthChanged);
	}
}

//////////////////////////////////////////////////////////////////////////
// Attribute Delegates

void ANDDPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	// Update Health here or in the HUD itself. 
	// Probably prefer updating it in the HUD itself? 
	int x = 0;
}

void ANDDPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	int x = 0;
}

