// CopyRight 2016 Marco Arjona

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


private:

	// How far ahead of the player can we reach in cm
	float Reach = 100.0f;

	// A component to handle physics
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// A component to handle input basics
	UInputComponent* InputComponent = nullptr;

private:

	// The action mapping  to grab something
	void Grab();

	// The action mapping to release what we grabbed
	void Release();

	// Find attached physics handle
	void FindPhysicsComponent();
	
	// Look for attached input component and setup it
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
