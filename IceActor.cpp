// Fill out your copyright notice in the Description page of Project Settings.

#include "IceActor.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AIceActor::AIceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;
}

// Called when the game starts or when spawned
void AIceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIceActor::OnOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

