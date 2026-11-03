// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ASCoin::ASCoin()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp->SetGenerateOverlapEvents(true);
}

void ASCoin::BeginPlay()
{
    Super::BeginPlay();

    MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASCoin::OnMeshBeginOverlap );
}

void ASCoin::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsActive)
    {
        return;
    }

    ASCharacter* Player = Cast<ASCharacter>(OtherActor);

    if (Player)
    {
        ASPlayerState* PS = Player->GetPlayerState<ASPlayerState>();

        if (PS)
        {
            bIsActive = false;

            PS->AddCredits(CreditReward);
            if (CollectSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, CollectSound, GetActorLocation());
            }

            UE_LOG(LogTemp, Warning, TEXT("Coin Collected! Credits Awarded: %d"), CreditReward);

            Destroy();
        }
    }
}
