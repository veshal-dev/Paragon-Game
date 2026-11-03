// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EnvironmentQuery\EnvQueryManager.h"
//#include "EnvironmentQuery\EnvQueryTypes.h"
#include "EnvironmentQuery\EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"


static TAutoConsoleVariable<bool>CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable Spawning of Bots Via Timer"),ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	PlayerStateClass = ASPlayerState::StaticClass();
	
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);


	//Spawn Items

	if (SpawnItemQuery)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnItemQueryCompleted);
		}
	}

}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bots Spawning Disabled via 'CVarSpawnBots'."));
		return;
	}

	int32 NoOfBotsAlive = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		//if (Bot->IsAlive())
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NoOfBotsAlive++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i Bots Alive"), NoOfBotsAlive);


	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NoOfBotsAlive >= MaxBotCount)
	{

		UE_LOG(LogTemp, Log, TEXT("At Maximun Bot Found. Skipping Bot Spawn"));

		return;
	}


	UEnvQueryInstanceBlueprintWrapper* QueryInstance =  UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
	
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed"));
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{

		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(),Locations[0],50.0f,20,FColor::Blue,false,60.0f);
	}
}



void ASGameModeBase::KillAll()
{
	
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		//if (Bot->IsAlive())
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->KillActor(this);
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* PlayerKiller = Cast<ASCharacter>(Killer);
	ASAICharacter* Minion = Cast<ASAICharacter>(VictimActor);

	if (PlayerKiller&&Minion)
	{
		ASPlayerState* PS = PlayerKiller->GetPlayerState<ASPlayerState>();

		if (PS)
		{
			PS->AddCredits(10);

			UE_LOG(LogTemp, Warning, TEXT("Credits Awarded. Current Credits : %d"), PS->GetCredits());
		}
	}
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer : %s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
}


void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnItemQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	UE_LOG(LogTemp, Warning, TEXT("Item EQS Found %d Locations"), Locations.Num());

	if (!CoinClass || !HealthPotionClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Classes Not Assigned!"));
		return;
	}

	int32 CoinsSpawned = 0;
	int32 PotionsSpawned = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn Coins
	for (int32 i = 0; i < CoinSpawnCount && Locations.Num() > 0; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector SpawnLocation = Locations[RandomIndex];

		Locations.RemoveAtSwap(RandomIndex);

		AActor* SpawnedCoin = GetWorld()->SpawnActor<AActor>(CoinClass,SpawnLocation,FRotator::ZeroRotator,SpawnParams);

		if (SpawnedCoin)
		{
			CoinsSpawned++;
		}
	}

	// Spawn Health Potions
	for (int32 i = 0; i < HealthPotionSpawnCount && Locations.Num() > 0; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector SpawnLocation = Locations[RandomIndex];

		Locations.RemoveAtSwap(RandomIndex);

		AActor* SpawnedPotion = GetWorld()->SpawnActor<AActor>(HealthPotionClass,SpawnLocation,FRotator::ZeroRotator,SpawnParams);

		if (SpawnedPotion)
		{
			PotionsSpawned++;
		}
	}

	UE_LOG(LogTemp,Warning,TEXT("Items Spawned - Coins: %d | Health Potions: %d"),CoinsSpawned,PotionsSpawned);
}