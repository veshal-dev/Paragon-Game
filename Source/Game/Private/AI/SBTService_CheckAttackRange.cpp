// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "BehaviorTree/BlackBoardComponent.h"
#include "AIController.h"


void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	// Want to find the Distance Between the AI Pawn and the Target Pawn.....

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor= Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 2000.f;

					bool bHasLOS = false;
					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}
				

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}


}
