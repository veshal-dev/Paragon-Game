// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;


}

// Called when the game starts or when spawned
void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("Health Potion Interacted"));

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);

	if (AttributeComp && !AttributeComp->IsFullHealth() && bIsActive)
	{
		AttributeComp->ApplyHealthChange(+50.0f);

		
		MeshComp->SetVisibility(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bIsActive = false;
		GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &ASHealthPotion::ReactivatePotion, 10.0f);
	}
}

void ASHealthPotion::ReactivatePotion()
{
	bIsActive = true;

	MeshComp->SetVisibility(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	
}