// AlphaDecodeX Studios Copyright 2022

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick. bCanEverTick = true;
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
	if(PhysicsHandle == nullptr){
		// Physics Handle is Found
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

	// UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed!!"));
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	// Try and Reach any Actors that has a Physics Collison channel set
	// If we hit something then attach physics handle
	// Only Raycast when we press the key
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	if(HitResult.GetActor()){
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);
	}
}

FVector UGrabber::GetPlayersReach()const{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation 
	);
	return PlayerViewPointLocation + Reach * PlayerViewPointRotation.Vector();
}

void UGrabber::Release()
{
	// UE_LOG(LogTemp, Warning, TEXT("Grabber Released!!"));
	// Remove the physics handle
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation 
	);
	FVector LineTraceEnd = PlayerViewPointLocation + Reach * PlayerViewPointRotation.Vector();
	
	if(PhysicsHandle->GrabbedComponent){
		PhysicsHandle->SetTargetLocation(
			GetPlayersReach()
		);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()const{
	FHitResult Hit;
	//Logging out to test --> Anything that intersects
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation 
	);
	return PlayerViewPointLocation;
}