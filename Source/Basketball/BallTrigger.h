#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallTrigger.generated.h"

UCLASS()
class BASKETBALL_API ABallTrigger : public AActor
{
	GENERATED_BODY()

public:
	ABallTrigger();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Trigger;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                             AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp,
	                             int32 OtherBodyIndex,
	                             bool bFromSweep,
	                             const FHitResult& SweepResult);
};
