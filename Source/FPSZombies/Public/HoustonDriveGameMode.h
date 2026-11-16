#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HoustonDriveGameMode.generated.h"

UCLASS()
class FPSZOMBIES_API AHoustonDriveGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHoustonDriveGameMode();

	virtual APawn* SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
};
