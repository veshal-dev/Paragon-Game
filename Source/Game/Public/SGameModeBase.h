// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EnvironmentQuery\EnvQueryTypes.h"

#include "SGameModeBase.generated.h"


class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

class UCurveFloat;
/**
 * 
 */
UCLASS()
class GAME_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	ASGameModeBase();

	virtual void StartPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;


	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);


	UPROPERTY(EditDefaultsOnly, Category = "Items")
	UEnvQuery* SpawnItemQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<AActor> CoinClass;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<AActor> HealthPotionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	int32 CoinSpawnCount = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	int32 HealthPotionSpawnCount = 3;

	UFUNCTION()
	void OnItemQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus);




public:
	UFUNCTION(Exec)
	void KillAll();


	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

};
