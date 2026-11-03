// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SItemBase.h"
#include "SItemChest.generated.h"


class USoundBase;


UCLASS()
class GAME_API ASItemChest : public ASItemBase
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintCallable, Category = "Chest")
	bool TryOpenChest(APawn* InstigatorPawn);



protected:

	

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ChestLidMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chest")
	float LidTurnRotationValue=120.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
    int32 CreditReward = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chest")
    bool bIsOpened = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* ChestOpenSound;

};
