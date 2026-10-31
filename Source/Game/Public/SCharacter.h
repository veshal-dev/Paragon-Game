// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"


#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;

class UInputAction;
class UInputMappingContext;

class USInteractionComponent;
class UAnimMontage;

class USAttributeComponent;

class UMaterialInstanceDynamic;
class UParticleSystem;




UENUM(BlueprintType)
enum class ETurnInPlace : uint8
{
	None,
	Left90,
	Left180,
	Right90,
	Right180
};

UCLASS()
class GAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UInputAction* PrimaryAttackAction;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PrimaryInteraction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* BlackHoleAction;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY()
	UMaterialInstanceDynamic* HitFlashMaterial;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TimerHandle_Dash;

public:

	void Move(const FInputActionValue& Value);
	void UpdateCharacterRotation(float DeltaTime);

	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);

	void PrimaryAttack();
	void PrimaryAttack_TimeElasped();
	void PrimaryInteract();

	void BlackholeAttack();
	void BlackHoleAttack_TimeElapsed();

	void Dash();
	void Dash_TimeElapsed();


protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DashProjectileClass;


	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* MagicCastEffect;

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

private:

	
	UPROPERTY(EditAnywhere, Category = "Character|Rotation")
	float MovementRotationInterpSpeed = 8.0f;

	
	void CheckForTurnInPlace();

	UPROPERTY(EditAnywhere, Category = "Character|Turn In Place")
	float TurnInPlaceThreshold = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Character|Turn In Place")
	float Turn180Threshold = 135.0f;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Character|Turn In Place",meta = (AllowPrivateAccess = "true"))
	ETurnInPlace CurrentTurnRequest = ETurnInPlace::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Character|Turn In Place",meta = (AllowPrivateAccess = "true"))
	bool bIsTurningInPlace = false;

public:

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);


};
