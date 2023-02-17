// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	RootComponent = BallMesh;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(GetRootComponent());
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnComponentHit);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnComponentBeginOverlap);
	SetReady(true);
}

void ABall::SetReady(const bool NewBool)
{
	bReady = NewBool;
}


void ABall::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
	{
		return;
	}

	if (Sound && BallMesh->GetPhysicsLinearVelocity().Size() > 170.f)
	{
		BallMesh->SetPhysicsLinearVelocity(BallMesh->GetPhysicsLinearVelocity() / 1.102f);
		BallMesh->SetPhysicsAngularVelocityInDegrees(BallMesh->GetPhysicsAngularVelocityInDegrees() / 1.122f);
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation(), FMath::RandRange(0.4f, 0.6f),
		                                      FMath::RandRange(1.f, 1.05f));
	}
}

void ABall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
}
