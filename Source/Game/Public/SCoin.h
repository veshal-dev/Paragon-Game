// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SItemBase.h"
#include "SCoin.generated.h"

class UPrimitiveComponent;
class USoundBase;

UCLASS()
class GAME_API ASCoin : public ASItemBase
{
	GENERATED_BODY()


public:

    ASCoin();

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Credits")
    int32 CreditReward = 5;

    UFUNCTION()
    void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);


    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundBase* CollectSound;
	
};
