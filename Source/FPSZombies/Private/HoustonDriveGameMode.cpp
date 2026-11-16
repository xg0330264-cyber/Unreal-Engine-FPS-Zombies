#include "HoustonDriveGameMode.h"

#include "HoustonDrivePawn.h"
#include "Engine/World.h"

AHoustonDriveGameMode::AHoustonDriveGameMode()
{
	DefaultPawnClass = AHoustonDrivePawn::StaticClass();
}

APawn* AHoustonDriveGameMode::SpawnDefaultPawnAtTransform(
	AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	FTransform GroundedTransform = SpawnTransform;
	if (UWorld* World = GetWorld())
	{
		const FVector Start = SpawnTransform.GetLocation() + FVector(0.0f, 0.0f, 500.0f);
		const FVector End = Start - FVector(0.0f, 0.0f, 5000.0f);
		FHitResult Hit;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(DriveSpawnGround), false);
		if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
		{
			GroundedTransform.SetLocation(FVector(
				SpawnTransform.GetLocation().X,
				SpawnTransform.GetLocation().Y,
				Hit.ImpactPoint.Z));
		}
	}
	return Super::SpawnDefaultPawnAtTransform(NewPlayer, GroundedTransform);
}
