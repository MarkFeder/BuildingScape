// CopyRight 2016 Marco Arjona

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Open the door after satisfying the condition
	void OpenDoor();
	
	// Close the door after satisfying the condition
	void CloseDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
private:

	// The angle of the door
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = 45.0f;

	// The pressure plate the door is attached to
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	// The delay of the open door
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 5.0f;

	// The time that specifies when the door was opened
	float LastDoorOpenTime;

	// The Actor that opens the door
	AActor* ActorThatOpens;

	// The Owner of this object
	AActor* Owner;

	// Handler to manage the time
	FTimerHandle TimerHandle;

private:

	UFUNCTION()
	void TriggerEnter(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void TriggerExit(AActor* OverlappedActor, AActor* OtherActor);

	float GetTotalMassOfActorsOnPlate() const;
};
