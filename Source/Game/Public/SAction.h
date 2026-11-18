// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;

UCLASS(Blueprintable)
class GAME_API USAction : public UObject
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditDefaultsOnly,Category="Actions")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent,Category="Actions")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Actions")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent,Category="Action")
	bool CanStart(AActor* Instiagtor);

	UFUNCTION(BlueprintCallable,Category="Action")
	bool IsRunning() const;

	UPROPERTY(EditDefaultsOnly,Category="Action")
	bool bAutoStart;

protected:

	

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditAnywhere,Category="Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable,Category="Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditAnywhere,Category="Tags")
	FGameplayTag SummaTag;

	bool bIsRunning;
};
