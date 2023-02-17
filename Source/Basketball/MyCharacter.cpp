#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Ball.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetCapsuleComponent());
}

AMyCharacter* AMyCharacter::Instance;

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 350.f;
	CharacterMovementComponent->PushForceFactor = 1000.0f;
	CharacterMovementComponent->MaxTouchForce = 100.0f;
	Instance = this;
}

void AMyCharacter::StartRun()
{
	bRunning = true;

	CharacterMovementComponent->MaxWalkSpeed = 450.f;
}

void AMyCharacter::Run()
{
	if (bRunning)
	{
		if (!bGoingForward)
		{
			return;
		}
		const float Time = UGameplayStatics::GetTimeSeconds(GetWorld());
		constexpr static float Amplitude = 5.f;
		constexpr static float Frequency = 20.f;
		FVector CameraRelativeLocation = Camera->GetRelativeLocation();
		CameraRelativeLocation.Z += Amplitude * FMath::Sin(Time * Frequency);
		Camera->SetRelativeLocation(CameraRelativeLocation);
	}
}

void AMyCharacter::StopRun()
{
	bRunning = false;
	CharacterMovementComponent->MaxWalkSpeed = 350.f;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Camera, FVector(60, 0, 90),
	                                      Camera->GetRelativeRotation(), true, true, 0.25f, false,
	                                      EMoveComponentAction::Move, LatentInfo);
}

void AMyCharacter::GetTheBall()
{
	if (TempBall)
	{
		CurrentBall = TempBall;
		CurrentBall->SetReady(false);
		CurrentBall->BallMesh->SetSimulatePhysics(false);
		CurrentBall->AttachToComponent(Camera, FAttachmentTransformRules::KeepWorldTransform);

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(CurrentBall->GetRootComponent(), FVector(150, 0, -10),
		                                      FRotator::ZeroRotator, true, true, 0.25f, false,
		                                      EMoveComponentAction::Move, LatentInfo);
	}
}

void AMyCharacter::DropTheBall()
{
	if (CurrentBall)
	{
		CurrentBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentBall->BallMesh->SetSimulatePhysics(true);
		CurrentBall->BallMesh->ComponentVelocity = FVector::ZeroVector;
		CurrentBall = {};
		TempBall = {};
	}
}

void AMyCharacter::BallDetect()
{
	if (CurrentBall)
	{
		return;
	}
	const APlayerCameraManager* const PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FHitResult OutHit;
	FVector Start = PlayerCameraManager->GetCameraLocation();
	FVector ForwardVector = PlayerCameraManager->GetCameraLocation().ForwardVector.GetSafeNormal();
	ForwardVector = PlayerCameraManager->GetCameraRotation().Vector();
	FVector End = ((ForwardVector * 300.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Camera, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			if (ABall* Ball = Cast<ABall>(OutHit.GetActor()))
			{
				if (Ball->GetReady())
				{
					DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 1, 30, FColor::Green, false, 0.1);

					TempBall = Ball;
				}
			}
			else
			{
				DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 1, 30, FColor::Red, false, 0.1);

				TempBall = {};
			}
		}
	}
}

void AMyCharacter::StartShoot()
{
	if (CurrentBall)
		bShootingStarted = true;
}

void AMyCharacter::Shoot()
{
	if (CurrentBall && bShootingStarted && Power < MaxPower)
	{
		Power += IncreaseAmount;
	}
}

void AMyCharacter::StopShoot()
{
	bShootingStarted = false;
	if (CurrentBall && Power > 0.f)
	{
		CurrentBall->BallMesh->SetSimulatePhysics(true);
		CurrentBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentBall->BallMesh->AddForce(Camera->GetForwardVector().GetSafeNormal() * Power + FVector::UpVector * 40);
		DropTheBall();
		Power = {};
		CurrentBall = {};
		TempBall = {};
	}
}

void AMyCharacter::Cancel()
{
	Power = {};
	bShootingStarted = false;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BallDetect();
	Shoot();
	Run();
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &AMyCharacter::GetTheBall);
	PlayerInputComponent->BindKey(EKeys::Q, IE_Pressed, this, &AMyCharacter::DropTheBall);
	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AMyCharacter::StartShoot);
	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Released, this, &AMyCharacter::StopShoot);
	PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &AMyCharacter::Cancel);
	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Pressed, this, &AMyCharacter::StartRun);
	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Released, this, &AMyCharacter::StopRun);
	PlayerInputComponent->BindAxis("Horizontal", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Vertical", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("HorizontalRotation", this, &AMyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("VerticalRotation", this, &AMyCharacter::LookUpAtRate);
}

void AMyCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value > 0) { bGoingForward = true; }
	if (Value <= 0)
	{
		bGoingForward = false;
	}
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value);
}

AMyCharacter* AMyCharacter::GetInstance()
{
	return Instance;
}
