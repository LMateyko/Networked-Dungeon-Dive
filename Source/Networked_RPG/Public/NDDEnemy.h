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

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	//TSubclassOf<class UNDDFloatingStatusBarWidget> UIFloatingStatusBarClass;

	//UPROPERTY()
	//class UNDDFloatingStatusBarWidget* UIFloatingStatusBar;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	//class UWidgetComponent* UIFloatingStatusBarComponent;

	virtual void BeginPlay() override;

	FDelegateHandle HealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
