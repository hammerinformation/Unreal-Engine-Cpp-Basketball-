#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class BASKETBALL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

private:
	UCharacterMovementComponent* CharacterMovementComponent{};
	UPROPERTY(VisibleAnywhere)
	class ABall* CurrentBall{};
	UPROPERTY(VisibleAnywhere)
	class ABall* TempBall{};
	void StartRun();
	void Run();
	void StopRun();
	void GetTheBall();
	void DropTheBall();
	void BallDetect();
	void Shoot();
	void StartShoot();
	void StopShoot();
	void Cancel();

	UPROPERTY(EditAnywhere)
	bool bShootingStarted{};

	UPROPERTY(EditAnywhere)
	float IncreaseAmount{};

	UPROPERTY(EditAnywhere)
	float MaxPower{};

	UPROPERTY(EditAnywhere)
	float Power{};

	UPROPERTY(VisibleAnywhere)
	float bRunning{};

	UPROPERTY(VisibleAnywhere)
	bool bGoingForward{};

	static AMyCharacter* Instance;
public:
	UFUNCTION(BlueprintCallable, BlueprintGetter)
	inline float GetPower() const { return Power; }

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveRight(float Value);
	void MoveForward(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	static AMyCharacter* GetInstance();
};
