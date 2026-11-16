#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoustonDriveEnvironment.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class FPSZOMBIES_API AHoustonDriveEnvironment : public AActor
{
	GENERATED_BODY()

public:
	AHoustonDriveEnvironment();

private:
	UPROPERTY(VisibleAnywhere, Category = "City")
	TObjectPtr<UInstancedStaticMeshComponent> Ground;

	UPROPERTY(VisibleAnywhere, Category = "City")
	TObjectPtr<UInstancedStaticMeshComponent> Roads;

	UPROPERTY(VisibleAnywhere, Category = "City")
	TObjectPtr<UInstancedStaticMeshComponent> Buildings;
};
