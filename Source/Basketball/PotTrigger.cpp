#include "PotTrigger.h"

#include "Ball.h"
#include "Components/BoxComponent.h"

APotTrigger::APotTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
}

void APotTrigger::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APotTrigger::OnComponentBeginOverlap);
}

void APotTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	if (const ABall* Ball = Cast<ABall>(OtherActor))
	{
		Ball->BallMesh->SetPhysicsLinearVelocity(Ball->BallMesh->GetPhysicsLinearVelocity() / Amount);
	}
}
