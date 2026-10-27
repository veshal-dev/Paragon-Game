// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTaskNode_Heal.h"



#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Pawn.h"


EBTNodeResult::Type UMyBTTaskNode_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		return EBTNodeResult::Failed;
	}

	APawn* MyPawn = AIC->GetPawn();
	if (!MyPawn)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
	if (!AttributeComp)
	{
		return EBTNodeResult::Failed;
	}

	AttributeComp->RestoreHealthToMax();

	return EBTNodeResult::Succeeded;

	//return EBTNodeResult::Type();



}
