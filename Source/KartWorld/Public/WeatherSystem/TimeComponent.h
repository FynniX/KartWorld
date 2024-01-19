// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "SkyCreatorActor.h"
#include "TimeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KARTWORLD_API UTimeComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Is the parent of the actor component which will be controlled.
	UPROPERTY()
	ASkyCreator* Parent;

	// Is the timeline which handles time.
	UPROPERTY()
	FTimeline Timeline;

public:	
	// Duration of one day (in minutes)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Options", meta = (ClampMin = "0.01", ClampMax = "2880"))
	float DurationOfDay = 1440.0f;

	// Sets default values for this component's properties
	UTimeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called on property change
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Called on time progress.
	UFUNCTION()
	void TimeProgress(float Time);

	// Called on day ended.
	UFUNCTION()
	void DayEnded();
};
