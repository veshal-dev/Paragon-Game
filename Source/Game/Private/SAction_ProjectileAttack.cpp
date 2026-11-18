// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework\Character.h"
#include "Kismet/GameplayStatics.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackAnimDelay = 0.01f;
	HandSocketName = "Muzzle_01";

}



void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect,Character->GetMesh(),HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"AttackDelay_Elasped",Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}

	

}




void USAction_ProjectileAttack::AttackDelay_Elasped(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector GunLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(1.0);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);



		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		FVector TraceTarget = TraceStart +
			(InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		FVector AimPoint = TraceTarget;

		UE_LOG(LogTemp, Warning, TEXT("Trace Start: %s"),
			*TraceStart.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Trace End: %s"),
			*TraceTarget.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Control Rotation: %s"),
			*InstigatorCharacter->GetControlRotation().ToString());

		if (GetWorld()->SweepSingleByObjectType(
			Hit,
			TraceStart,
			TraceTarget,
			FQuat::Identity,
			ObjParams,
			Shape,
			Params))
		{
			AimPoint = Hit.ImpactPoint;

			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"),
				*GetNameSafe(Hit.GetActor()));

			UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"),
				*Hit.Location.ToString());

			UE_LOG(LogTemp, Warning, TEXT("Impact Point: %s"),
				*Hit.ImpactPoint.ToString());

			UE_LOG(LogTemp, Warning, TEXT("Impact Normal: %s"),
				*Hit.ImpactNormal.ToString());

			UE_LOG(LogTemp, Warning, TEXT("Start Penetrating: %s"),
				Hit.bStartPenetrating ? TEXT("TRUE") : TEXT("FALSE"));

			UE_LOG(LogTemp, Warning, TEXT("Hit Distance: %f"),
				Hit.Distance);
		}

		FVector ShotDirection = (AimPoint - GunLocation).GetSafeNormal();

		if (ShotDirection.IsNearlyZero())
		{
			return;
		}

		FRotator ProjRotation = ShotDirection.Rotation();

		FTransform SpawnTM = FTransform(ProjRotation, GunLocation);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		//**************************************

		UE_LOG(LogTemp, Warning, TEXT("Control Rotation: %s"),
			*InstigatorCharacter->GetControlRotation().ToString());

		UE_LOG(LogTemp, Warning, TEXT("Trace Target: %s"),
			*TraceTarget.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Aim Point: %s"),
			*AimPoint.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Gun Location: %s"),
			*GunLocation.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Shot Direction: %s"),
			*ShotDirection.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Projectile Rotation: %s"),
			*ProjRotation.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Pawn View Location: %s"),
			*InstigatorCharacter->GetPawnViewLocation().ToString());

		UE_LOG(LogTemp, Warning, TEXT("Actor Location: %s"),
			*InstigatorCharacter->GetActorLocation().ToString());

		UE_LOG(LogTemp, Warning, TEXT("Actor Rotation: %s"),
			*InstigatorCharacter->GetActorRotation().ToString());

		UE_LOG(LogTemp, Warning, TEXT("Control Rotation: %s"),
			*InstigatorCharacter->GetControlRotation().ToString());


	}

	StopAction(InstigatorCharacter);
}


