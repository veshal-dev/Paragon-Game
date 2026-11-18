// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "GameplayTagContainer.h"
#include "SPlayerMagicProjectile.generated.h"

class UAudioComponent;
class USoundBase;
class UCameraShakeBase;
class USActionEffect;


UCLASS()
class GAME_API ASPlayerMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:
	ASPlayerMagicProjectile();


protected:

    virtual void OnActorHit(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComp;



	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	USoundBase* FlightSound;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase>ImpactCameraShake;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag ParryTag;

	// Stores the information needed to resume the projectile after parrying.
	FVector PendingParryVelocity;

	TWeakObjectPtr<APawn> PendingParryInstigator;

	void ResumeParriedProjectile();

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TSubclassOf<USActionEffect> BurningActionClass;


};
