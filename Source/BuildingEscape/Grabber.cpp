// AlphaDecodeX Studios Copyright 2022

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle){
		// Physics Handle is Found
	}else{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component is Found on : %s"), *GetOwner()->GetName());
	}
	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		UE_LOG(LogTemp, Warning, TEXT("Input Component is found %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}


}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed!!"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// Get the Players ViewPoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation 
	);
	// UE_LOG(LogTemp, Warning, TEXT("Player Location is : %f %f %f"), 
	// PlayerViewPointLocation.X,PlayerViewPointLocation.Y, PlayerViewPointLocation.Z );
	// UE_LOG(LogTemp, Warning, TEXT("Player Rotation is : %f %f %f"), 
	// PlayerViewPointRotation.Roll, PlayerViewPointRotation.Pitch, PlayerViewPointRotation.Yaw);
	
	// UE_LOG(LogTemp, Warning, TEXT("Player Location is : %s and Rotation is %s"), 
	// *PlayerViewPointLocation.ToString(), 
	// *PlayerViewPointRotation.ToString());

	//	Draw a line to show the Player Reach
	FVector LineTraceEnd = PlayerViewPointLocation + Reach * PlayerViewPointRotation.Vector();
	DrawDebugLine(
			GetWorld(),
			PlayerViewPointLocation,
			LineTraceEnd,
			FColor(0,255,0),
			false,
			0.f,
			0,
			5.f
	);

	FHitResult Hit;
	//Logging out to test --> Anything that intersects
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	if(ActorHit){
		UE_LOG(LogTemp, Warning, TEXT("Line Trace has hit : %s"), *(ActorHit->GetName()));
	}

	// Ray Casting at certain distance (Reach)
	// See what It hits 

}

