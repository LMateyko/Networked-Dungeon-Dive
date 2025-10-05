// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttachedUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API UAttachedUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Find the root canvas panel (expected in a 'main hud' style widget) and add new widget as child. */
	UFUNCTION(BlueprintCallable, Category = UI)
	static void AddToRootCanvasPanel(UUserWidget* InNewWidget);

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "UI", meta = (ExposeOnSpawn = true))
	TObjectPtr<AActor> AttachedActor;

	// TODO: Pivot to a more status related Widget class. 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> ParentOverlay;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;

	bool bWasOnScreen = true;
};
