// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta,MaxHealth);

	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}

	return nullptr;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health >= MaxHealth;
}
