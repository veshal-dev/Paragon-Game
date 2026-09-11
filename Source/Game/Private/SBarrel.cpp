// Fill out your copyright notice in the Description page of Project Settings.


#include "SBarrel.h"
#include "SMagicProjectile.h"
#include "DrawDebugHelpers.h"



// Sets default values
ASBarrel::ASBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BarrelMesh -> SetGenerateOverlapEvents(true);
	RootComponent = BarrelMesh;
	BarrelMesh->SetSimulatePhysics(true);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(BarrelMesh);

	RadialForce->SetAutoActivate(false);
	RadialForce->Radius = 600.0f;
	RadialForce->ImpulseStrength = 2000.0f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);

}

// Called when the game starts or when spawned
void ASBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BarrelMesh->OnComponentHit.AddDynamic(this, &ASBarrel::OnHit);
}

// Called every frame
void ASBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBarrel::OnHit(
	UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, 
	const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("BARREL HIT!"));

	if (OtherActor && OtherActor->IsA(ASMagicProjectile::StaticClass()) && OtherActor->ActorHasTag("Projectile"))
	{
		UE_LOG(LogTemp, Warning, TEXT("PROJECTILE HIT BARREL - FIRING IMPULSE!"));

		RadialForce->FireImpulse();

		UE_LOG(LogTemp, Warning, TEXT("Actor Hit"));
		UE_LOG(LogTemp, Warning, TEXT("Actor Name : %s, At Time : %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit Location : %s"),*Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(),Hit.ImpactPoint,CombinedString,nullptr,FColor::Green,2.0f,true);
	} 
	
}

void ASBarrel::Explode()
{
	RadialForce->FireImpulse();
}

