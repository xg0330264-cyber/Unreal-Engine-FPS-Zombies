#include "HoustonDrivePawn.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HoustonDriveEnvironment.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AHoustonDrivePawn::AHoustonDrivePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* VehicleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VehicleRoot"));
	SetRootComponent(VehicleRoot);

	CollisionBody = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBody"));
	CollisionBody->SetupAttachment(VehicleRoot);
	CollisionBody->SetBoxExtent(FVector(205.0f, 92.0f, 52.0f));
	CollisionBody->SetRelativeLocation(FVector(0.0f, 0.0f, 56.0f));
	CollisionBody->SetCollisionProfileName(TEXT("Pawn"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(
		TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SteelMaterial(
		TEXT("/Game/StarterContent/Materials/M_Metal_Steel.M_Metal_Steel"));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBody"));
	BodyMesh->SetupAttachment(VehicleRoot);
	BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 56.0f));
	BodyMesh->SetRelativeScale3D(FVector(4.0f, 1.8f, 0.72f));
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CubeMesh.Succeeded())
	{
		BodyMesh->SetStaticMesh(CubeMesh.Object);
	}
	if (SteelMaterial.Succeeded())
	{
		BodyMesh->SetMaterial(0, SteelMaterial.Object);
	}

	const FVector WheelLocations[] = {
		FVector(128.0f, -91.0f, 28.0f),
		FVector(128.0f, 91.0f, 28.0f),
		FVector(-128.0f, -91.0f, 28.0f),
		FVector(-128.0f, 91.0f, 28.0f)
	};
	for (int32 Index = 0; Index < UE_ARRAY_COUNT(WheelLocations); ++Index)
	{
		UStaticMeshComponent* Wheel = CreateDefaultSubobject<UStaticMeshComponent>(
			*FString::Printf(TEXT("Wheel%d"), Index));
		Wheel->SetupAttachment(VehicleRoot);
		Wheel->SetRelativeLocation(WheelLocations[Index]);
		Wheel->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		Wheel->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.18f));
		Wheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (CylinderMesh.Succeeded())
		{
			Wheel->SetStaticMesh(CylinderMesh.Object);
		}
		WheelMeshes.Add(Wheel);
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(VehicleRoot);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 206.0f));
	CameraBoom->TargetArmLength = 720.0f;
	CameraBoom->SetRelativeRotation(FRotator(-12.0f, 0.0f, 0.0f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 4.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AHoustonDrivePawn::BeginPlay()
{
	Super::BeginPlay();

	if (!UGameplayStatics::GetActorOfClass(GetWorld(), AHoustonDriveEnvironment::StaticClass()))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AHoustonDriveEnvironment>(
			AHoustonDriveEnvironment::StaticClass(),
			GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams);
	}
}

void AHoustonDrivePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateVehicle(DeltaSeconds);
}

void AHoustonDrivePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("DriveThrottle"), this, &AHoustonDrivePawn::SetThrottle);
	PlayerInputComponent->BindAxis(TEXT("DriveSteer"), this, &AHoustonDrivePawn::SetSteering);
	PlayerInputComponent->BindAction(TEXT("DriveBrake"), IE_Pressed, this, &AHoustonDrivePawn::SetBrake, true);
	PlayerInputComponent->BindAction(TEXT("DriveBrake"), IE_Released, this, &AHoustonDrivePawn::SetBrake, false);
}

void AHoustonDrivePawn::SetThrottle(float Value)
{
	ThrottleInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AHoustonDrivePawn::SetSteering(float Value)
{
	SteeringInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AHoustonDrivePawn::SetBrake(bool bPressed)
{
	bBrakePressed = bPressed;
}

void AHoustonDrivePawn::UpdateVehicle(float DeltaSeconds)
{
	if (bBrakePressed)
	{
		CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, 0.0f, DeltaSeconds, BrakeDeceleration);
	}
	else if (!FMath::IsNearlyZero(ThrottleInput))
	{
		const float DesiredSpeed = ThrottleInput * MaxSpeed;
		const float Rate = FMath::Sign(CurrentSpeed) != FMath::Sign(DesiredSpeed)
			? BrakeDeceleration
			: Acceleration;
		CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, DesiredSpeed, DeltaSeconds, Rate);
	}
	else
	{
		CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, 0.0f, DeltaSeconds, CoastDeceleration);
	}

	const float SpeedRatio = FMath::Clamp(FMath::Abs(CurrentSpeed) / MaxSpeed, 0.15f, 1.0f);
	if (!FMath::IsNearlyZero(SteeringInput) && !FMath::IsNearlyZero(CurrentSpeed))
	{
		const float ReverseDirection = CurrentSpeed < 0.0f ? -1.0f : 1.0f;
		AddActorWorldRotation(FRotator(
			0.0f,
			SteeringInput * MaxSteeringRate * SpeedRatio * ReverseDirection * DeltaSeconds,
			0.0f));
	}

	if (!FMath::IsNearlyZero(CurrentSpeed))
	{
		FHitResult Hit;
		AddActorWorldOffset(GetActorForwardVector() * CurrentSpeed * DeltaSeconds, true, &Hit);
		if (Hit.IsValidBlockingHit())
		{
			CurrentSpeed = 0.0f;
		}
	}
}
