// CopyRight 2016 Marco Arjona

#include "BuildingScape.h"
#include "Grabber.h"

#define OUT

UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		// Input component is found
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()));
	}
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Look for attached Physics handle
	FindPhysicsComponent();

	// Look for attached Input component
	SetupInputComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	#pragma region DrawDebugLineAndTest

	//// Log out to test
	//UE_LOG(LogTemp, Warning, TEXT("PlayerViewPointLocation : %s"), *PlayerViewPointLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("PlayerViewPointRotation : %s"), *PlayerViewPointRotator.ToString());


	//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor::Red, false, 0.0f, 0.0f, 5.0f);
#pragma endregion 

	// Ray-cast (AKA line-trace) out to reach distance
	FHitResult LineTraceHit;
	FCollisionObjectQueryParams LineTraceObjectQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams LineTraceQueryParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType
	(
		OUT LineTraceHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		LineTraceObjectQueryParams,
		LineTraceQueryParams
	);

	return LineTraceHit;
}

void UGrabber::Grab()
{
	// Try and reach any actors with physics body collision channer set
	auto HitResult = GetFirstPhysicsBodyInReach();

	// If we hit something then attach a physics handle
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		PhysicsHandle->GrabComponent
		(
			ComponentToGrab, 
			NAME_None, 
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button released!"));

	PhysicsHandle->ReleaseComponent();
}

// Refactoring code: first lines of GetReachLineStart() and GetReachLineEnd()
// by making use of OUT parameters or returning a struct

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotator);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotator);

	// Calculate trace in the world
	FVector LineTraceDirection = PlayerViewPointRotator.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + (LineTraceDirection * Reach);

	return LineTraceEnd;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

