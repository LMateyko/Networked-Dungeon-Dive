// Fill out your copyright notice in the Description page of Project Settings.


#include "NDDPlayerState.h"
#include "NDDAbilitySystemComponent.h"

ANDDPlayerState::ANDDPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UNDDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* ANDDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
