// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "Behaviortree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"




// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//PawnSensingComp->OnSeePawn.AddDynamic(this,&ASAICharacter::OnPawnSeen);

	//AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);

	UE_LOG(LogTemp, Warning, TEXT("Step 1: PostInitializeComponents started"));

	if (IsValid(PawnSensingComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Step 2: PawnSensingComp is valid"));

		PawnSensingComp->OnSeePawn.AddUniqueDynamic(this,&ASAICharacter::OnPawnSeen);
		
	}

	AttributeComp = FindComponentByClass<USAttributeComponent>();

	// Register the health-change event.
	if (IsValid(AttributeComp))
	{
		AttributeComp->OnHealthChanged.AddUniqueDynamic(this,&ASAICharacter::OnHealthChanged);
	
	}


}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);

	DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER SPOTTED",nullptr,FColor::Blue,2.0f,true,1.0f);
	
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta<0.0f)
	{
		if (InstigatorActor!=this)
		{
			SetTargetActor(InstigatorActor);
		}


		if (ActiveHealthWidget == nullptr)
		{
			ActiveHealthWidget = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthWidget)
			{
				ActiveHealthWidget->AttachedActor = this;
				ActiveHealthWidget->AddToViewport();
			}
		}
		


		if (NewHealth<=0.0f)
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* TargetActor)
{

	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
		
	}

}





