// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerMagicProjectile.h"

#include "SPlayerMagicProjectile.h"
#include "SAttributeComponent.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"



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


void ASPlayerMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit)
{
    if (OtherActor == GetInstigator())
    {
        return;
    }

    if (OtherActor)
    {
        USAttributeComponent* AttributeComp =Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));

        if (AttributeComp)
        {
            AttributeComp->ApplyHealthChange(Damage);
        }
    }

    if (ImpactCameraShake)
    {
        UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, Hit.ImpactPoint, 0.0f, 1000.0f, 1.0f);
    }

    Super::OnActorHit(HitComponent,OtherActor,OtherComp,NormalImpulse,Hit);
}


