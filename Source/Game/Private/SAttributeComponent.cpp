// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;	
	}
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
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

bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent* AttributeComp = GetAttributes(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return  false;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health >= MaxHealth;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void USAttributeComponent::RestoreHealthToMax()
{
	const float Delta = MaxHealth - Health;

	if (Delta <= 0.0f)
	{
		return;
	}

	Health = MaxHealth;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
}

bool USAttributeComponent::KillActor(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}
