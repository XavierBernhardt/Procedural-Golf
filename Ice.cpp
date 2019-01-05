// Fill out your copyright notice in the Description page of Project Settings.

#include "Ice.h"


// Sets default values for this component's properties
UIce::UIce()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	//RootComponent = SceneRoot;
	// ...
}


// Called when the game starts
void UIce::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UIce::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UIce::OnOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

