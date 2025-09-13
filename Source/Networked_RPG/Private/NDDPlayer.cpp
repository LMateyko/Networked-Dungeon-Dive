// Copyright 2025 Lucas Mateyko


#include "NDDPlayer.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "../Networked_RPG.h"

#include "NDDAbilitySystemComponent.h"
#include "NDDAttributeSetBase.h"
#include "NDDPlayerState.h"
#include "NDDPlayerController.h"

ANDDPlayer::ANDDPlayer()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ANDDPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ANDDPlayerState* playerState = GetPlayerState<ANDDPlayerState>();
	if (playerState)
	{
		AbilitySystemComponent = Cast<UNDDAbilitySystemComponent>(playerState->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(playerState, this);

		LogOnScreen(this, "InitAbilityActorInfo - PossessedBy", FColor::Blue, 10.0f);

		InitializeAttributes();

		// Only add abilities on the server. Bind input on both SetupPlayerInputComponent() and OnRep_PlayerState()
		AddCharacterAbilities();

		ANDDPlayerController* PC = Cast<ANDDPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
	}
}

// Client Only
void ANDDPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ANDDPlayerState* PS = GetPlayerState<ANDDPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UNDDAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		LogOnScreen(this, "InitAbilityActorInfo - OnRep_PlayerState", FColor::Blue, 10.0f);

		InitializeAttributes();

		ANDDPlayerController* PC = Cast<ANDDPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input and Bindings

void ANDDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ANDDPlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ANDDPlayer::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANDDPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANDDPlayer::Look);

		// Bind all Ability Input
		for (const FNDDAbilityInputToInputActionBinding& binding : AbilityInputBindings)
		{
			EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Triggered, this, &ANDDPlayer::AbilityInputBindingPressed, binding.AbilityTag);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANDDPlayer::AbilityInputBindingPressed(FGameplayTagContainer AbilityTag)
{
	LogOnScreen(this, "AbilityInputBindingPressed ", FColor::Green, 2.0f);
	AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTag, true);
}

void ANDDPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ANDDPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANDDPlayer::ApplyDefaultAttributesToEffectContext(FGameplayEffectContextHandle EffectContext)
{
	Super::ApplyDefaultAttributesToEffectContext(EffectContext);

	FGameplayEffectSpecHandle defaultStateHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultState, 1, EffectContext);
	if (defaultStateHandle.IsValid())
	{
		FActiveGameplayEffectHandle AbilityModeEffectSpecHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*defaultStateHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}
