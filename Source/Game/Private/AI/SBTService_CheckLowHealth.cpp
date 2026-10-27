// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "SAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC)
	{
		APawn* MyPawn = AIC->GetPawn();
		if (MyPawn)
		{
			USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
			if (AttributeComp)
			{
				float CurrentHealth = AttributeComp->GetHealth();
				float MaxHealth = AttributeComp->GetMaxHealth();
				if (MaxHealth>0)
				{
					float HealthPercentage = CurrentHealth / MaxHealth;

					bool bIsLowHealth = HealthPercentage <= 0.3f;

					UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
					if (BlackboardComp)
					{
						BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowHealth);
					}
				}

			}

		}
	}



}
