#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HoustonDrivePawn.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS()
class FPSZOMBIES_API AHoustonDrivePawn : public APawn
{
	GENERATED_BODY()

public:
	AHoustonDrivePawn();

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	void SetThrottle(float Value);
	void SetSteering(float Value);
	void SetBrake(bool bPressed);
	void UpdateVehicle(float DeltaSeconds);

	UPROPERTY(VisibleAnywhere, Category = "Vehicle")
	TObjectPtr<UBoxComponent> CollisionBody;

	UPROPERTY(VisibleAnywhere, Category = "Vehicle")
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Vehicle")
	TArray<TObjectPtr<UStaticMeshComponent>> WheelMeshes;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Vehicle", meta = (ClampMin = "100.0"))
	float MaxSpeed = 2800.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle", meta = (ClampMin = "100.0"))
	float Acceleration = 1800.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle", meta = (ClampMin = "1.0"))
	float CoastDeceleration = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle", meta = (ClampMin = "100.0"))
	float BrakeDeceleration = 4200.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle", meta = (ClampMin = "1.0"))
	float MaxSteeringRate = 105.0f;

	float ThrottleInput = 0.0f;
	float SteeringInput = 0.0f;
	float CurrentSpeed = 0.0f;
	bool bBrakePressed = false;
};
