// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "SAttributeComponent.h"




USBTTaskNode_RangedAttack::USBTTaskNode_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type USBTTaskNode_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn==nullptr)
		{
			return EBTNodeResult::Failed;
		}



		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor==nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f,MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(0.0f,MaxBulletSpread);


		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
		

		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;


	}

	return	EBTNodeResult::Failed; 
	//return EBTNodeResult::Type();
}
