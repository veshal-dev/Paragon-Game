// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerMagicProjectile.h"

#include "SPlayerMagicProjectile.h"
#include "SAttributeComponent.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SGameplayFunctionLibrary.h"
#include "SActionComponent.h"

#include "GameFramework\ProjectileMovementComponent.h"
#include "Components\SphereComponent.h"

#include "SActionEffect.h"



ASPlayerMagicProjectile::ASPlayerMagicProjectile()
{
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
    AudioComp->SetupAttachment(RootComponent);
 
}


void ASPlayerMagicProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    AudioComp->SetSound(FlightSound);
    AudioComp->Play();
}


void ASPlayerMagicProjectile::ResumeParriedProjectile()
{
    APawn* NewInstigator = PendingParryInstigator.Get();

    if (!IsValid(NewInstigator))
    {
        return;
    }

    // Stop ignoring the original shooter.
    APawn* PreviousInstigator = GetInstigator();

    if (IsValid(PreviousInstigator))
    {
        SphereComp->IgnoreActorWhenMoving(PreviousInstigator, false);
    }

    // Update the projectile's instigator.
    SetInstigator(NewInstigator);

    // Ignore the new shooter so the projectile doesn't immediately hit them.
    SphereComp->IgnoreActorWhenMoving(NewInstigator, true);

    // Reattach the movement component to the projectile collision component.
    ProjectileComp->SetUpdatedComponent(SphereComp);

    // Apply the reversed velocity.
    ProjectileComp->Velocity = PendingParryVelocity;

    // Reactivate projectile movement.
    ProjectileComp->Activate(true);

    // Update the component's velocity.
    ProjectileComp->UpdateComponentVelocity();

    UE_LOG(LogTemp, Warning, TEXT("PARRY MOVEMENT RESUMED!"));
}

void ASPlayerMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit)
{
    if (OtherActor == GetInstigator())
    {
        return;
    }

    //static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");


    USActionComponent* ActionComp =
        OtherActor->FindComponentByClass<USActionComponent>();

    if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
    {
        // Store the new direction.
        PendingParryVelocity = -ProjectileComp->Velocity;

        // Store the new instigator.
        PendingParryInstigator = Cast<APawn>(OtherActor);

        UE_LOG(LogTemp, Warning, TEXT("PARRY DETECTED!"));

        // Resume movement after Unreal finishes processing this hit.
        GetWorldTimerManager().SetTimerForNextTick(this,&ASPlayerMagicProjectile::ResumeParriedProjectile);

        return;
    }
    

     USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, Hit);
    

    if (ImpactCameraShake)
    {
        UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, Hit.ImpactPoint, 0.0f, 1000.0f, 1.0f);
    }

    Super::OnActorHit(HitComponent,OtherActor,OtherComp,NormalImpulse,Hit);

    if (ActionComp)
    {
        ActionComp->AddAction(GetInstigator(), BurningActionClass);
    }
}


