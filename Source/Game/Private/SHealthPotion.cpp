// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
    
    if (!bIsActive || !InstigatorPawn)
    {
        return;
    }

    USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
    
    if (!AttributeComp || AttributeComp->IsFullHealth())
    {
        return;
    }

    ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
    if (!PS)
    {
        return;
    }

    if (PS->TrySpendCredits(CreditCost))
    {
        AttributeComp->ApplyHealthChange(this, 20.0f);

        Deactivate();

        UE_LOG(LogTemp, Warning, TEXT("Health Potion Used! Credits Remaining: %d"),PS->GetCredits());
    }

    else
    {
        UE_LOG(LogTemp, Warning,TEXT("Not Enough Credits! Required: %d | Available: %d"),CreditCost,PS->GetCredits());
    }
    
    
    
    
    
    
    
    
    /*USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);

    if (AttributeComp && !AttributeComp->IsFullHealth() && bIsActive)
    {
        AttributeComp->ApplyHealthChange(this, 20.0f);

        Deactivate();
    }*/
}


//void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("Health Potion Interacted"));
//
//	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
//
//	if (AttributeComp && !AttributeComp->IsFullHealth() && bIsActive)
//	{
//		AttributeComp->ApplyHealthChange(this,+50.0f);
//
//		
//		MeshComp->SetVisibility(false);
//		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		bIsActive = false;
//		GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &ASHealthPotion::ReactivatePotion, 10.0f);
//	}
//}
//
//void ASHealthPotion::ReactivatePotion()
//{
//	bIsActive = true;
//
//	MeshComp->SetVisibility(true);
//	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	
//}