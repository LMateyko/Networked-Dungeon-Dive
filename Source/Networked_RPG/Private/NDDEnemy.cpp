// Copyright 2025 Lucas Mateyko

#include "NDDEnemy.h"

#include "NDDAbilitySystemComponent.h"
#include "NDDAttributeSetBase.h"
#include "AttachedUserWidget.h"
#include "Kismet/GameplayStatics.h"

ANDDEnemy::ANDDEnemy()
{
	// Create ability system component, and set it to be explicitly replicated
	EnemyAbilitySystemComponent = CreateDefaultSubobject<UNDDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EnemyAbilitySystemComponent->SetIsReplicated(true);

	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	EnemyAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Set our parent's TWeakObjectPtr
	AbilitySystemComponent = EnemyAbilitySystemComponent;

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	EnemyAttributeSetBase = CreateDefaultSubobject<UNDDAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set our parent's TWeakObjectPtr
	AttributeSetBase = EnemyAttributeSetBase;
}

void ANDDEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddCharacterAbilities();

		// Setup FloatingStatusBar UI for Locally Owned Players only, not AI or the server's copy of the PlayerControllers
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->IsLocalPlayerController())
		{
			// Create once, and skip on instant kill
			if (ActiveHealthBar == nullptr)
			{
				ActiveHealthBar = CreateWidget<UAttachedUserWidget>(GetWorld(), HealthBarWidgetClass);
				if (ActiveHealthBar)
				{
					ActiveHealthBar->AttachedActor = this;
					// Throws Error due to missing main canvas? 
					UAttachedUserWidget::AddToRootCanvasPanel(ActiveHealthBar);
				}
			}
		}

		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ANDDEnemy::HealthChanged);

	}
}

void ANDDEnemy::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	//// Update floating status bar
	//if (UIFloatingStatusBar)
	//{
	//	UIFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
	//}

	//// If the minion died, handle death
	//if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	//{
	//	Die();
	//}
}
