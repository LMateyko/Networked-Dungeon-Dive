// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "NDDCharacter.h"

#include "NDDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API ANDDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 
	void CreateHUD();

	class UNDD_HUD_Widget* GetHUD();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UNDD_HUD_Widget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UNDD_HUD_Widget* UIHUDWidget;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
