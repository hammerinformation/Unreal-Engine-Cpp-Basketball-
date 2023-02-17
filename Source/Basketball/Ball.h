#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class BASKETBALL_API ABall : public AActor
{
	GENERATED_BODY()

public:
	ABall();

protected:
	virtual void BeginPlay() override;
private:
	bool bReady{};
public:
	inline bool GetReady() const { return bReady; }
	UFUNCTION(BlueprintCallable)
	void SetReady(const bool NewBool);

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditAnywhere)
	class USoundBase* Sound;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere)
	USoundConcurrency* SoundConcurrency;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                             AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp,
	                             int32 OtherBodyIndex,
	                             bool bFromSweep,
	                             const FHitResult& SweepResult);
};
