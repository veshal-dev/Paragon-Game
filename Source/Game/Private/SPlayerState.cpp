// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"




void ASPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState,Credits);
}

void ASPlayerState::OnRep_Credits()
{
	OnCreditsChanged.Broadcast(Credits);
}

void ASPlayerState::AddCredits(int32 Amount)
{
	if (!HasAuthority() || Amount<=0)
	{
		return;
	}

	Credits += Amount;
	OnCreditsChanged.Broadcast(Credits);

	//Update
	ForceNetUpdate();
}

bool ASPlayerState::TrySpendCredits(int32 Amount)
{
	if (!HasAuthority() || Amount<=0 || Credits	< Amount)
	{
		return false;
	}

	Credits -= Amount;
	OnCreditsChanged.Broadcast(Credits);

	//Update
	ForceNetUpdate();
	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}
