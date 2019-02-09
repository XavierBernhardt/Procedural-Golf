// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeNodeMain.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
//AMazeNodeMain::AMazeNodeMain(int type, int floor):type(type),floor(floor)

AMazeNodeMain::AMazeNodeMain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	type = 0;
	floor = 0;

}	
//Types:
//0 1 2 3 4 5 6
//C I L N T X S
//Default: C/0
//type = 0;
// 6 / S = SAND

//Floor:
//0			1		2		4
//Grass		Ice		Hole	Sand
//Default			(unused)
//floor = 0;

//-1 just generates nothing for floor / wall 

// Called when the game starts or when spawned
//void AMazeNodeMain::BeginPlay()
//{
//	Super::BeginPlay();
//}

void AMazeNodeMain::Destroyed()
{
	if (myPiece != nullptr)
	myPiece->Destroy();
	if (myFloor != nullptr)
	myFloor->Destroy();

}

//// Called every frame
//void AMazeNodeMain::Tick(float DeltaTime) //might be able to disable this
//{
//	Super::Tick(DeltaTime);
//
//}

void AMazeNodeMain::setType(int setType)
{
	type = setType;
}

void AMazeNodeMain::setFloor(int setFloor)
{
	floor = setFloor;
}

void AMazeNodeMain::init()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (type) {
	case 0: //C
		if (!DiceRoll(ChanceForAlt2C))
			myPiece = GetWorld()->SpawnActor<AActor>(MazeC, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myPiece = GetWorld()->SpawnActor<AActor>(MazeCAlt2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 1: //I
		if (!DiceRoll(ChanceForAlt2I))
			myPiece = GetWorld()->SpawnActor<AActor>(MazeI, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myPiece = GetWorld()->SpawnActor<AActor>(MazeIAlt2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 2: //L
		if (!DiceRoll(ChanceForAlt2L))
			myPiece = GetWorld()->SpawnActor<AActor>(MazeL, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myPiece = GetWorld()->SpawnActor<AActor>(MazeLAlt2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 3: //N
		if (!DiceRoll(ChanceForAlt2N))
			myPiece = GetWorld()->SpawnActor<AActor>(MazeN, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myPiece = GetWorld()->SpawnActor<AActor>(MazeNAlt2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 4: //T
		if (!DiceRoll(ChanceForAlt2T))
			myPiece = GetWorld()->SpawnActor<AActor>(MazeT, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myPiece = GetWorld()->SpawnActor<AActor>(MazeTAlt2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 5: //X
		if (!DiceRoll(ChanceForAlt2X))
			myPiece = GetWorld()->SpawnActor<AActor>(MazeX, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myPiece = GetWorld()->SpawnActor<AActor>(MazeXAlt2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 6: //S(and)
		myPiece = GetWorld()->SpawnActor<AActor>(MazeWallSAND, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	break;
	}
	switch (floor) {
	case 0: //flat
		myFloor = GetWorld()->SpawnActor<AActor>(MazeFloor, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 2: //hole
		myFloor = GetWorld()->SpawnActor<AActor>(MazeFloorHole, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 4: //Sand
		if (!DiceRoll(ChanceForSandFloorAlt))
			myFloor = GetWorld()->SpawnActor<AActor>(MazeFloorSAND, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myFloor = GetWorld()->SpawnActor<AActor>(MazeFloorSAND2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	}
}

bool AMazeNodeMain::DiceRoll(int percentage)
{
	if (percentage == 0) { //prevent dividing by 0
		return false;
	}
	return (FMath::RandRange(1, 100 / percentage) == 1 ? true : false);
}