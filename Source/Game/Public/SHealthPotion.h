// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SItemBase.h"

#include "SGameplayInterface.h"

#include "SHealthPotion.generated.h"

UCLASS()
class GAME_API ASHealthPotion : public ASItemBase
{
	GENERATED_BODY()

public:	
	
	ASHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
    int32 CreditCost = 20;

};
