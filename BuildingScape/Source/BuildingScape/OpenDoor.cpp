// CopyRight 2016 Marco Arjona

#include "BuildingScape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UOpenDoor::TriggerEnter(AActor* OverlappedActor, AActor* OtherActor)
{
	FString OverlappedActorName = OverlappedActor->GetName();
	FString OtherActorName = OtherActor->GetName();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("The name of OverlappedActor is : %s"), *OverlappedActorName));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("The name of OtherActor is : %s"), *OtherActorName));

	OpenDoor();
}

void UOpenDoor::TriggerExit(AActor* OverlappedActor, AActor* OtherActor)
{
	FString OverlappedActorName = OverlappedActor->GetName();
	FString OtherActorName = OtherActor->GetName();


	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("The name of OverlappedActor is : %s"), *OverlappedActorName));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("The name of OtherActor is : %s"), *OtherActorName));


	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UOpenDoor::CloseDoor, DoorCloseDelay, false, -1.0f);
}

void UOpenDoor::OpenDoor()
{
	// Create a rotator
	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);

	// Set the door rotation
	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	// Create a rotator
	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);
	
	// Set the door rotation
	Owner->SetActorRotation(NewRotation);
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Find the owning actor
	Owner = GetOwner();

	// Find the first player actor
	APawn *FirstActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if (FirstActor)
	{
		ActorThatOpens = FirstActor;
	}

	// Register callback events
	PressurePlate->OnActorBeginOverlap.AddDynamic(this, &UOpenDoor::TriggerEnter);
	PressurePlate->OnActorEndOverlap.AddDynamic(this, &UOpenDoor::TriggerExit);
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//// Poll the Trigger Volume
	//if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	//{
	//	OpenDoor();
	//	LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	//}

	//// Check if it's time to close the door
	//float CurrentTime = GetWorld()->GetTimeSeconds();

	//if (CurrentTime - LastDoorOpenTime > DoorCloseDelay)
	//{
	//	CloseDoor();
	//}
}

