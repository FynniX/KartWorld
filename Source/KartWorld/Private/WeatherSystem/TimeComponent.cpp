// Copyright @ 2023 Fynn Haupt


#include "WeatherSystem/TimeComponent.h"

// Sets default values for this component's properties
UTimeComponent::UTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Parent = Cast<ASkyCreator>(GetOwner());
	if (!Parent) UE_LOG(LogTemp, Error, TEXT("Parent actor isn't a sky creator actor!"));

	// Curve for time timeline (0.0f Solar Time to 23.99f Solar Time).
	UCurveFloat* TimeCurveNormal = CreateDefaultSubobject<UCurveFloat>(TEXT("Time Curve Normal"));
	TimeCurveNormal->FloatCurve.UpdateOrAddKey(0.f, 0.f);
	TimeCurveNormal->FloatCurve.UpdateOrAddKey(23.99f, 23.99f);

	// Bind of function for TimeCurveNormal on update.
	FOnTimelineFloat TimeProgressFunction;
	TimeProgressFunction.BindUFunction(this, TEXT("TimeProgress"));

	// Bind of function for event which fires on day end.
	FOnTimelineEvent DayEndedFunction;
	DayEndedFunction.BindUFunction(this, TEXT("DayEnded"));

	// Setup timeline for time handling.
	Timeline.AddInterpFloat(TimeCurveNormal, TimeProgressFunction);
	Timeline.AddEvent(23.99f, DayEndedFunction);
	Timeline.SetTimelineLengthMode(TL_LastKeyFrame);
	Timeline.SetLooping(true);
}

// Called when the game starts
void UTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!Parent) return;
	Timeline.SetPlaybackPosition(Parent->EditorTimeOfDay, false, false);
	Timeline.SetPlayRate(1.0f / (DurationOfDay * 60.0f) * 24.0f);
	Timeline.Play();
}

void UTimeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Timeline.SetPlayRate(1.0f / (DurationOfDay * 60.0f) * 24.0f);
}

void UTimeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!Parent) return;
	Timeline.Stop();
}

// Called every frame
void UTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Timeline.TickTimeline(DeltaTime);
}

void UTimeComponent::TimeProgress(float Time)
{
	if (!Parent) return;
	Parent->SetTime(Time);
}

void UTimeComponent::DayEnded()
{
	if (!Parent) return;

	int32 Day = Parent->Day;
	int32 Month = Parent->Month;
	int32 Year = Parent->Year;

	// Is day + 1 in range of current month
	if (Day + 1 <= FDateTime::DaysInMonth(Year, Month)) {
		Parent->SetDay(Day + 1);
		return;
	}

	// Reset Day
	Parent->SetDay(1);

	// Is month + 1 smaller then / equals 12
	if (Month + 1 <= 12) {
		Parent->SetMonth(Month + 1);
		return;
	}

	// Reset Month
	Parent->SetMonth(1);

	// Add year
	Parent->SetYear(Year + 1);
}