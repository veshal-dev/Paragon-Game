// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditAnywhere, Category = "Teleport")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:

	ASDashProjectile();
	
};
