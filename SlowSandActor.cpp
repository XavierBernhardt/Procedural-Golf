// Fill out your copyright notice in the Description page of Project Settings.

#include "SlowSandActor.h"

// Sets default values
ASlowSandActor::ASlowSandActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;
}

// Called when the game starts or when spawned
void ASlowSandActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlowSandActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlowSandActor::OnOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}
