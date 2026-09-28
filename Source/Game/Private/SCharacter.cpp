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

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent= Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASCharacter::Sprint);
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

void ASCharacter::PrimaryAttack()
{ 
	PlayAnimMontage(AttackAnim);
	
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElasped,0.01f);
	//GetWorldTimerManager().ClearTimer(TimerHandle_Name);

	
}


//void ASCharacter::PrimaryAttack_TimeElasped()
//{
//	if (ensure(ProjectileClass))
//	{
//		FVector GunLocation = GetMesh()->GetSocketLocation("Muzzle_01");
//		FRotator GunRotation = GetMesh()->GetSocketRotation("Muzzle_01");
//
//		FTransform SpawnTM = FTransform(GetControlRotation(), GunLocation);
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		SpawnParams.Instigator = this;
//
//		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
//	}
//
//}


void ASCharacter::PrimaryAttack_TimeElasped()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}

void ASCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector GunLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);



		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();

		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() + 5000);

		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - GunLocation).Rotator();

		FTransform SpawnTM = FTransform(GetControlRotation(), GunLocation);

		UGameplayStatics::SpawnEmitterAttached(MagicCastEffect,GetMesh(),TEXT("Muzzle_01"));

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta,float MaxHealth)
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


