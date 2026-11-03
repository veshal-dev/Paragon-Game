// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "SPlayerState.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"



// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChestLidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLidMesh"));
	ChestLidMesh->SetupAttachment(MeshComp);

	LidTurnRotationValue = 120.0f;

}

bool ASItemChest::TryOpenChest(APawn* InstigatorPawn)
{
	if (bIsOpened || !InstigatorPawn)
	{
		return false;
	}

	ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();

	if (!PS)
	{
		return false;
	}

	bIsOpened = true;

	PS->AddCredits(CreditReward);

	UE_LOG(LogTemp, Warning,TEXT("Treasure Chest Opened! Credits Awarded: %d | Total Credits: %d"),CreditReward,PS->GetCredits());

	if (ChestOpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ChestOpenSound,GetActorLocation());
	}

	return true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn) 
{
	TryOpenChest(InstigatorPawn);
}




