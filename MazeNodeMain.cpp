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
//0 1 2 3 4 5
//C I L N T X 
//Default: C/0
//type = 0;
// 6 = SAND
// 7 = Cave
// 8 = I ramp
// 9 = L ramp

//Floor:
//0			1		2		3		4		5		6
//Grass		Ice		Hole	Cave	Sand	I ramp	L ramp
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
	case 6: //Sand
		myPiece = GetWorld()->SpawnActor<AActor>(MazeWallSAND, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 7: //Cave
		myPiece = GetWorld()->SpawnActor<AActor>(MazeWallCave, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 9: //I ramp
		myPiece = GetWorld()->SpawnActor<AActor>(IRampWall, GetActorLocation(), GetActorRotation(), spawnParams);
		if (inverted)
			myPiece->SetActorScale3D(FVector(-1.f, -1.f, 1.f));
		break;
	case 8: //L ramp
		if (inverted)
			myPiece = GetWorld()->SpawnActor<AActor>(LRampWallInverted, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myPiece = GetWorld()->SpawnActor<AActor>(LRampWall, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	break;
	}
	switch (floor) {
	case 0: //Flat
		myFloor = GetWorld()->SpawnActor<AActor>(MazeFloor, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 2: //Hole
		myFloor = GetWorld()->SpawnActor<AActor>(MazeFloorHole, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 3: //Cave
		myFloor = GetWorld()->SpawnActor<AActor>(MazeFloorCave, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 4: //Sand
		if (!DiceRoll(ChanceForSandFloorAlt))
			myFloor = GetWorld()->SpawnActor<AActor>(MazeFloorSAND, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myFloor = GetWorld()->SpawnActor<AActor>(MazeFloorSAND2, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	case 6: //I ramp
		myFloor = GetWorld()->SpawnActor<AActor>(IRampFloor, GetActorLocation(), GetActorRotation(), spawnParams);
		if (inverted)			
			myFloor->SetActorScale3D(FVector(-1.f, -1.f, 1.f));
		break;
	case 5: //L ramp
		if (inverted)
			myFloor = GetWorld()->SpawnActor<AActor>(LRampFloorInverted, GetActorLocation(), GetActorRotation(), spawnParams);
		else
			myFloor = GetWorld()->SpawnActor<AActor>(LRampFloor, GetActorLocation(), GetActorRotation(), spawnParams);
		break;
	}
	//if (inverted) {


	//	if (type == 8) { //L pieces need to be rotated 180 degrees
	//		myFloor->SetActorScale3D(FVector(1.f, 1.f, 1.f));
	//		myPiece->SetActorScale3D(FVector(1.f, 1.f, 1.f));
	//		myFloor->SetActorRotation(FRotator(0.f, 180.f, 0.f));
	//		myPiece->SetActorRotation(FRotator(0.f, 180.f, 0.f));
	//	}
	//	else{
	//		myFloor->SetActorScale3D(FVector(-1.f, -1.f, 1.f));
	//		myPiece->SetActorScale3D(FVector(-1.f, -1.f, 1.f));
	//	}
	//}
}

bool AMazeNodeMain::DiceRoll(int percentage)
{
	if (percentage == 0) { //prevent dividing by 0
		return false;
	}
	return (FMath::RandRange(1, 100 / percentage) == 1 ? true : false);
}

void AMazeNodeMain::invert()
{
	inverted = true;
}
