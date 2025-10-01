// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "NDDCharacter.h"


#include "NDDEnemy.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API ANDDEnemy : public ANDDCharacter
{
	GENERATED_BODY()
	
public:
	ANDDEnemy();

protected:
	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY()
	TObjectPtr<class UNDDAbilitySystemComponent> EnemyAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY()
	TObjectPtr<class UNDDAttributeSetBase> EnemyAttributeSetBase;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HealthBarWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<class UAttachedUserWidget> ActiveHealthBar;

	virtual void BeginPlay() override;

	FDelegateHandle HealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
