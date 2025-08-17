// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NDD_HUD_Widget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API UNDD_HUD_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
* Attribute setters
*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);
};
