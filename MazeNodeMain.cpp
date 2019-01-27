// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeNodeMain.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AMazeNodeMain::AMazeNodeMain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Types:
	//0 1 2 3 4 5
	//C I L N T X
	//Default: C/0
	type = 0;

	//Floor:
	//0			1		2
	//Grass		Ice		Hole 
	//Default			(unused)
	floor = 0;

}	

// Called when the game starts or when spawned
void AMazeNodeMain::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (type) {
	case 0: //C
		GetWorld()->SpawnActor<AActor>(MazeC, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 1: //I
		GetWorld()->SpawnActor<AActor>(MazeI, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 2: //L
		GetWorld()->SpawnActor<AActor>(MazeL, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 3: //N
		GetWorld()->SpawnActor<AActor>(MazeN, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 4: //T
		GetWorld()->SpawnActor<AActor>(MazeT, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 5: //X
		GetWorld()->SpawnActor<AActor>(MazeX, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	
	break;
	}

	switch (floor) {
	case 0: //C
		GetWorld()->SpawnActor<AActor>(MazeFloor, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	}

}

// Called every frame
void AMazeNodeMain::Tick(float DeltaTime) //might be able to disable this
{
	Super::Tick(DeltaTime);

}

