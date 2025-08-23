// Fill out your copyright notice in the Description page of Project Settings.


#include "NDDCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ANDDCharacter::ANDDCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

// Server Only
void ANDDCharacter::PossessedBy(AController* NewController)
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
void ANDDCharacter::OnRep_PlayerState()
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

void ANDDCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input and Bindings

void ANDDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANDDCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANDDCharacter::Look);

		// Bind all Ability Input
		for (const FNDDAbilityInputToInputActionBinding& binding : AbilityInputBindings)
		{
			EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Triggered, this, &ANDDCharacter::AbilityInputBindingPressed, binding.AbilityTag);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANDDCharacter::AbilityInputBindingPressed(FGameplayTagContainer AbilityTag)
{
	LogOnScreen(this, "AbilityInputBindingPressed ", FColor::Green, 2.0f);
	AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTag, true);
}

void ANDDCharacter::Move(const FInputActionValue& Value)
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

void ANDDCharacter::Look(const FInputActionValue& Value)
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

//////////////////////////////////////////////////////////////////////////
// Attributes and Abilities

UAbilitySystemComponent* ANDDCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ANDDCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	if (IsValid(AbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(AbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent.Get(), this));
	}

	LogOnScreen(this, "Adding Character Abilities", FColor::Red, 10.0f);
	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ANDDCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
		return;

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

bool ANDDCharacter::IsAlive() const
{
	float health = GetHealth();
	float maxHealth = GetMaxHealth();

	UE_LOG(LogTemp, Display, TEXT("%s current health: %f/%f"), *GetName(), health, maxHealth);

	return true;
}

float ANDDCharacter::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float ANDDCharacter::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

