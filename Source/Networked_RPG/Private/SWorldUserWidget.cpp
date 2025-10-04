// Copyright 2025 Lucas Mateyko


#include "SWorldUserWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SWorldUserWidget)

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenPosition))
	{
		float scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= scale;

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
