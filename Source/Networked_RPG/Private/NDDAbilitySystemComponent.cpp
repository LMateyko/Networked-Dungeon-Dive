// Copyright 2025 Lucas Mateyko


#include "NDDAbilitySystemComponent.h"

void UNDDAbilitySystemComponent::ReceiveDamage(UNDDAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
