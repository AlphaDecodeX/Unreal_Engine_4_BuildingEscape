#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();	

	// ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	FindAudioComponent();	
}

void UOpenDoor::FindPressurePlate(){
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());	
	}
}
// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	// FRotator OpenDoor(0.f, 0.f, 0.f);

	// OpenDoor.Yaw = FMath::Lerp(CurrentYaw, OpenAngle, 0.02f);
	// OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, 2	);
	if (TotalMassOfActors() > MassToOpenDoor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenDoor Function has been called"));
		OpenDoor(DeltaTime);
        DoorLastOpened = GetWorld()->GetTimeSeconds();
    }else{
        if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay){
                CloseDoor(DeltaTime);
        }
    }
    
	
}

void UOpenDoor::OpenDoor(float DeltaTime){
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is : %f"), GetOwner()->GetActorRotation().Yaw);

	
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = 	CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	CloseDoorSound = false;
	if(!AudioComponent){return;}
	if(!OpenDoorSound){
		AudioComponent->Play();
		OpenDoorSound = true;
	}
	
}

void UOpenDoor::CloseDoor(float DeltaTime){
    
    CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);
	OpenDoorSound = false;
	if(!AudioComponent){return;}
	
	if(!CloseDoorSound){
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors()const{

	if(!PressurePlate){
		UE_LOG(LogTemp, Warning, TEXT("Pressure Plate is Not Setuped so no opendoor"));
		return 0.f;
	}
	UE_LOG(LogTemp, Error, TEXT("I am not a Error"));
	// Find all overlapping Actors and calculate Total Mass
	float TotalMass = 0.f;	
	TArray<AActor*> OverlappingActors;
	if(!PressurePlate){return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Adding the masses
	for (AActor* Actor : OverlappingActors)
	{	
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::FindAudioComponent() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent){
		UE_LOG(LogTemp, Error, TEXT("Missing Audio Component"));
	}
}