// Copyright 2025 Lucas Mateyko

#pragma once

#include "CoreMinimal.h"
#include "NDDCharacter.h"

#include "NDDPlayer.generated.h"

USTRUCT()
struct FNDDAbilityInputToInputActionBinding
{
	GENERATED_BODY();

public:
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	struct FGameplayTagContainer AbilityTag;
};

/**
 * 
 */
UCLASS()
class NETWORKED_RPG_API ANDDPlayer : public ANDDCharacter
{
	GENERATED_BODY()

public:

	ANDDPlayer();

	// Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

protected:

	/* Player Input and Camera */
	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UCameraComponent> FollowCamera;

	// MappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	// Input Bindings For Abilities.
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<FNDDAbilityInputToInputActionBinding> AbilityInputBindings;

	// Gameplay Effect for setting the default state 
	// This is an infinite GE that sets the Primary Mode for using skills
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultState;

	/* Functions */
	// Client Only
	virtual void OnRep_PlayerState() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Bound to enhanced input to activate ability by tag
	void AbilityInputBindingPressed(struct FGameplayTagContainer AbilityTag);

	// Called for movement input
	void Move(const struct FInputActionValue& Value);

	// Called for looking input
	void Look(const struct FInputActionValue& Value);

	// Include the player's default Attack Mode with Attribute Initialization
	virtual void ApplyDefaultAttributesToEffectContext(struct FGameplayEffectContextHandle Context) override;
};
