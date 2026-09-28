// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SPlayerMagicProjectile.generated.h"

class UAudioComponent;
class USoundBase;
class UCameraShakeBase;


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


};
