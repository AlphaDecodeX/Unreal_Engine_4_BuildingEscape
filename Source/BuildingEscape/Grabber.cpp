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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle(){
	//Checking for the Physics Handle Object
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle){
		// Physics Handle is Found
	}else{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component is Found on : %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent(){
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed!!"));
	GetFirstPhysicsBodyInReach();
	// Try and Reach any Actors that has a Physics Collison channel set
	// If we hit something then attach physics handle
	// Only Raycast when we press the key
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released!!"));
	// Remove the physics handle
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()const{
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
	return Hit;

}