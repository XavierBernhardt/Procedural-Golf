// Fill out your copyright notice in the Description page of Project Settings.

#include "SpringboardActor.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ASpringboardActor::ASpringboardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;
}

// Called when the game starts or when spawned
void ASpringboardActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpringboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpringboardActor::OnOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}
