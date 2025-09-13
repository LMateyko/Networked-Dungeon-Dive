// Copyright 2025 Lucas Mateyko


#include "NDDPlayerController.h"

#include "AbilitySystemComponent.h"

#include "NDDCharacter.h"
#include "NDDPlayerState.h"
#include "NDD_HUD_Widget.h"


void ANDDPlayerController::CreateHUD()
{
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ANDDPlayerState* PS = GetPlayerState<ANDDPlayerState>();
	if (!PS)
	{
		return;
	}

	/*
	UIHUDWidget = CreateWidget<UNDD_HUD_Widget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport(0);

	// Set attributes
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	*/
}

UNDD_HUD_Widget* ANDDPlayerController::GetHUD()
{
	return UIHUDWidget;
}

// Server Only
void ANDDPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ANDDPlayerState* PS = GetPlayerState<ANDDPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void ANDDPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUD();
}

