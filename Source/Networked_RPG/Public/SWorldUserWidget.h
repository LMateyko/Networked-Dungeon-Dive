// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Transient, Category = "UI", meta = (ExposeOnSpawn = true))
	TObjectPtr<AActor> AttachedActor;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
