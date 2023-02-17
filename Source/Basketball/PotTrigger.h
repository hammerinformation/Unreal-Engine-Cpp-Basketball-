#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PotTrigger.generated.h"

UCLASS()
class BASKETBALL_API APotTrigger : public AActor
{
	GENERATED_BODY()

public:
	APotTrigger();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* Trigger{};
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                             AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp,
	                             int32 OtherBodyIndex,
	                             bool bFromSweep,
	                             const FHitResult& SweepResult);
public:
	UPROPERTY(EditAnywhere)
	float Amount{};
};
