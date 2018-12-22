// Fill out your copyright notice in the Description page of Project Settings.

#include "PC.h"
#include "Engine.h"


// Sets default values for this component's properties
UPC::UPC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPC::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Begin"));

	// ...
	
}


// Called every frame
void UPC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//void UPC::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{

//}