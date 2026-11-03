// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemBase.generated.h"

UCLASS()
class GAME_API ASItemBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemBase();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(EditDefaultsOnly, Category = "Item")
    float RespawnTime = 10.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    bool bIsActive = true;

    FTimerHandle TimerHandle_Respawn;

    void Deactivate();

    void Reactivate();

};
