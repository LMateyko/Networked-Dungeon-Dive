// Fill out your copyright notice in the Description page of Project Settings.


#include "NDDAbilitySystemComponent.h"

void UNDDAbilitySystemComponent::ReceiveDamage(UNDDAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
