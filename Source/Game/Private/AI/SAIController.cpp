// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackBoardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTreeAsset, TEXT("Behavior Tree is Nullptr, Please Check.....")))
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}


	/*APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this,0);
	if (MyPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}*/
}
