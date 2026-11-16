#include "HoustonDriveEnvironment.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AHoustonDriveEnvironment::AHoustonDriveEnvironment()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GrassMaterial(
		TEXT("/Game/StarterContent/Materials/M_Ground_Grass.M_Ground_Grass"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> RoadMaterial(
		TEXT("/Game/StarterContent/Materials/M_Concrete_Poured.M_Concrete_Poured"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BuildingMaterial(
		TEXT("/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall"));

	Ground = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Ground"));
	Ground->SetupAttachment(SceneRoot);
	Ground->SetCollisionProfileName(TEXT("BlockAll"));
	if (CubeMesh.Succeeded())
	{
		Ground->SetStaticMesh(CubeMesh.Object);
	}
	if (GrassMaterial.Succeeded())
	{
		Ground->SetMaterial(0, GrassMaterial.Object);
	}
	Ground->AddInstance(FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, -30.0f), FVector(125.0f, 125.0f, 0.35f)));

	Roads = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Roads"));
	Roads->SetupAttachment(SceneRoot);
	Roads->SetCollisionProfileName(TEXT("BlockAll"));
	if (CubeMesh.Succeeded())
	{
		Roads->SetStaticMesh(CubeMesh.Object);
	}
	if (RoadMaterial.Succeeded())
	{
		Roads->SetMaterial(0, RoadMaterial.Object);
	}

	constexpr float StreetSpacing = 2400.0f;
	constexpr float CityHalfLength = 10800.0f;
	for (int32 Street = -4; Street <= 4; ++Street)
	{
		const float Offset = Street * StreetSpacing;
		Roads->AddInstance(FTransform(FRotator::ZeroRotator, FVector(0.0f, Offset, -10.0f), FVector(CityHalfLength / 100.0f, 4.5f, 0.12f)));
		Roads->AddInstance(FTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(Offset, 0.0f, -10.0f), FVector(CityHalfLength / 100.0f, 4.5f, 0.12f)));
	}

	Buildings = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Buildings"));
	Buildings->SetupAttachment(SceneRoot);
	Buildings->SetCollisionProfileName(TEXT("BlockAll"));
	if (CubeMesh.Succeeded())
	{
		Buildings->SetStaticMesh(CubeMesh.Object);
	}
	if (BuildingMaterial.Succeeded())
	{
		Buildings->SetMaterial(0, BuildingMaterial.Object);
	}

	FRandomStream Random(5202);
	for (int32 BlockX = -4; BlockX < 4; ++BlockX)
	{
		for (int32 BlockY = -4; BlockY < 4; ++BlockY)
		{
			const FVector Location(
				(BlockX + 0.5f) * StreetSpacing + Random.FRandRange(-260.0f, 260.0f),
				(BlockY + 0.5f) * StreetSpacing + Random.FRandRange(-260.0f, 260.0f),
				0.0f);
			const FVector Scale(
				Random.FRandRange(3.2f, 5.8f),
				Random.FRandRange(3.2f, 5.8f),
				Random.FRandRange(5.0f, 16.0f));
			Buildings->AddInstance(FTransform(FRotator::ZeroRotator, Location + FVector(0.0f, 0.0f, Scale.Z * 50.0f), Scale));
		}
	}
}
