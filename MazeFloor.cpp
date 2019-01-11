// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeFloor.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMazeFloor::AMazeFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Maze Generation"));

	//Make a floor which spans the entire level (prevents "seam jumping" from ball vs floor seams)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorMesh(TEXT("/Game/Meshes/MazeFloorMesh.MazeFloorMesh"));
	MazeFloorObj = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MazeFloor_Generated"));
	MazeFloorObj->SetStaticMesh(FloorMesh.Object);
	FVector newScale = FVector(100.f, 100.f, 1.f); //this is where you make the floor scale with grid size
	MazeFloorObj->SetWorldScale3D(newScale);

}

// Called when the game starts or when spawned
void AMazeFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazeFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

