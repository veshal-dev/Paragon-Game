// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Delegates/DelegateCombinations.h"


#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChanged, int32, NewCredits);

UCLASS()
class GAME_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Credits",Replicated ,ReplicatedUsing = OnRep_Credits)
	int32 Credits = 0;

public:

	UFUNCTION(BlueprintCallable,Category="Credits")
	void AddCredits(int32 Amount);

	UFUNCTION(BlueprintCallable,Category="Credits")
	bool TrySpendCredits(int32 Amount);

	UFUNCTION(BlueprintPure,Category="Credits")
	int32 GetCredits() const;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Credits();

	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCreditsChanged OnCreditsChanged;
	
};
