// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Logging/LogMacros.h"

#include "NDDAbilitySet.h"

#include "NDDCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;

class UGameplayEffect;
class UGameplayAbility;

class UNDDAbilitySystemComponent;
class UNDDAttributeSetBase;

struct FInputActionValue;
struct FGameplayAbilitySpecHandle;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT()
struct FNDDAbilityInputToInputActionBinding
{
	GENERATED_BODY();

public:
	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	ENDDAbilityInput AbilityInput;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer AbilityTag;
};

//USTRUCT()
//struct FNDDAbilityInputBindings
//{
//	GENERATED_BODY();
//
//	UPROPERTY(EditDefaultsOnly, Category = "Input")
//	TArray<FNDDAbilityInputToInputActionBinding> Bindings;
//};

UCLASS(config = Game)
class NETWORKED_RPG_API ANDDCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UCameraComponent> FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	///** Basic Attack Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UInputAction> BasicAttackAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UAnimMontage> BasicAttackAnim;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	// Input Bindings For Abilities.
	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<FNDDAbilityInputToInputActionBinding> AbilityInputBindings;

public:
	ANDDCharacter();

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

protected:

	///** Called for basic attack input **/
	//void BasicAttack(const FInputActionValue& Value);

	void AbilityInputBindingPressed(FGameplayTagContainer AbilityTag);

	//void AbilityInputBindingPressedHandler(ENDDAbilityInput abilityInput);

	//void AbilityInputBindingReleasedHandler(ENDDAbilityInput abilityInput);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


protected:

	TWeakObjectPtr<UNDDAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UNDDAttributeSetBase> AttributeSetBase;

	bool AbilityBindingCompleted = false;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// To add mapping context
	virtual void BeginPlay();

	// Client Only
	virtual void OnRep_PlayerState() override;

	//UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Ability")
	//FGameplayTagContainer BasicAttackTag;

	//// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	//TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;

	// Binding of Ability to Inputs
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UNDDAbilitySet* AbilitySet;

	// Ability Handles granted by the ability set
	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();
	bool ASCInputBound = false;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};