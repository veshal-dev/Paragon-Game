// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera\CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "GameFramework\CharacterMovementComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "SInteractionComponent.h"
#include "SAttributeComponent.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));
	
	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));

}



void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

	HitFlashMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(13);


	if (HitFlashMaterial)
	{
		HitFlashMaterial->SetScalarParameterValue(TEXT("TimeToHit"), -1000.0f);
	}
}


void ASCharacter::CheckForTurnInPlace()
{
	if (!GetLastMovementInputVector().IsNearlyZero())
	{
		return;
	}

	const FRotator ControlRotation = GetControlRotation();
	const FRotator ActorRotation = GetActorRotation();

	const float YawDifference = FMath::FindDeltaAngleDegrees(ActorRotation.Yaw, ControlRotation.Yaw);

	if (FMath::Abs(YawDifference) < TurnInPlaceThreshold)
	{
		return;
	}
}



// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacterRotation(DeltaTime);
	CheckForTurnInPlace();

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent= Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASCharacter::SprintStop);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASCharacter::StartJump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASCharacter::StopJump);

		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &ASCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(PrimaryInteraction, ETriggerEvent::Started, this, &ASCharacter::PrimaryInteract);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ASCharacter::Dash);
		EnhancedInputComponent->BindAction(BlackHoleAction, ETriggerEvent::Started, this, &ASCharacter::BlackholeAttack);
		
	}

}

void ASCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	FRotator ControlRotation = GetControlRotation();
	
	

	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void ASCharacter::UpdateCharacterRotation(float DeltaTime)
{
	
	////FVector2D MovementVector = FVector2D::ZeroVector;
	//FRotator ControlRotation = GetControlRotation();

	//FRotator TargetRotation(0.0f, ControlRotation.Yaw, 0.0f);

	//if (GetLastMovementInputVector().IsNearlyZero())
	//{
	//	return;
	//}

	//FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 8.0f);

	//SetActorRotation(SmoothRotation);

	if (bIsTurningInPlace)
	{
		return;
	}
	const FVector MovementInput = GetLastMovementInputVector();
	if (MovementInput.IsNearlyZero())
	{
		return;
	}

	FRotator TargetRotation = MovementInput.Rotation();

	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;

	const FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, MovementRotationInterpSpeed);

	SetActorRotation(SmoothRotation);
}

void ASCharacter::Look(const FInputActionValue & Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
	
}

void ASCharacter::StartJump(const FInputActionValue & Value)
{
	Jump();
	
}

void ASCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void ASCharacter::Sprint(const FInputActionValue & Value)
{
	bool bIsSprinting = Value.Get<bool>();

	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}

}

// Interaction Component *************************************************************

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

// AttackComponent Begins *************************************************************

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackholeAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void ASCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

// AttackComponent Ends *************************************************************




void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{

	if (Delta < 0.0f)
	{
		if (HitFlashMaterial)
		{
			HitFlashMaterial->SetScalarParameterValue(TEXT("TimeToHit"), GetWorld()->GetTimeSeconds());
		}
	}

	if (NewHealth<=0.0f && Delta<0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		
	}
}


void ASCharacter::HealSelf(float Amount /*= 100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}


FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
	
	
	
}


