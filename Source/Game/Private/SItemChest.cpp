// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChestBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestBoxMesh"));
	RootComponent = ChestBoxMesh;

	ChestLidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLidMesh"));
	ChestLidMesh->SetupAttachment(ChestBoxMesh);

	LidTurnRotationValue = 120.0f;

}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	ChestLidMesh->SetRelativeRotation(FRotator(LidTurnRotationValue, 0, 0));
}

