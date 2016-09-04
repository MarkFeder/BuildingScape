// CopyRight 2016 Marco Arjona

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

public:
	
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnCloseRequest;
		
	// The angle of the door
	UPROPERTY(BlueprintReadWrite)
	float OpenAngle = 45.0f;

private:

	// The pressure plate the door is attached to
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	// The delay of the open door
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 5.0f;

	// The time that specifies when the door was opened
	float LastDoorOpenTime;

	// The Actor that opens the door
	AActor* ActorThatOpens = nullptr;

	// The Owner of this object
	AActor* Owner = nullptr;

	// Handler to manage the time
	FTimerHandle TimerHandle;

	// Minimum mass value to trigger the PressurePlate
	float TriggerMass = 60.0f;

private:

	UFUNCTION()
	void TriggerEnter(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void TriggerExit(AActor* OverlappedActor, AActor* OtherActor);

	float GetTotalMassOfActorsOnPlate() const;
};
