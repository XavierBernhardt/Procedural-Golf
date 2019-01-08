// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeCPP.h"
#include "Engine.h"
#include "Engine/World.h"
#include "PlayerPawn.h"
#include "vector"
#include "UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"

using namespace std;

AGameModeCPP::AGameModeCPP()
{
//constructor
}

void AGameModeCPP::InitGameState()
{
	Super::InitGameState();

	AGameState* MyGameState = Cast<AGameState>(GameState);
	if (MyGameState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("InitState"));

		FString LevelName = GetWorld()->GetMapName();
		LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

//		GEngine->AddOnScreenDebugMessage(-1, 99.0f, FColor::White, FString::Printf(TEXT("Level=%s"), *LevelName));


		if (LevelName.Equals("MazeGeneration")) {
			GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Maze Generation"));
			MazeGenerationBegin();
		}
		if (LevelName.Equals("ControlMap")) {
			GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Control Map"));
			
		}

		//CurrentHole = 0;

		////Code to fill the locations of each starting point
		////default set everything to 0.f
		//for (int i = 0; i < 10; i++) {
		//	x[i] = 0.f;
		//	y[i] = 0.f;
		//	z[i] = 0.f;
		//	r[i] = 380.f; //a rotation that will never be used -> used here to check if a hole here exists
		//	// eg if next r[currenthole+1] == 380.f then there is no hole there and we have ended.
		//}

		////Hole 0
		//x[0] = 5860.f;
		//y[0] = -8449.f;
		//z[0] = 555.f;
		//r[0] = 0.f;
		////Hole 1
		//x[1] = 8424.f;
		//y[1] = -3124.f;
		//z[1] = 555.f;
		//r[1] = 90.f;
		////Hole 2
		//x[2] = 8424.f;
		//y[2] = 2275.f;
		//z[2] = 555.f;
		//r[2] = 90.f;
		////Hole 3
		//x[3] = 2714.f;
		//y[3] = 8844.f;
		//z[3] = 555.f;
		//r[3] = 180.f;

		//MovePlayer();
	}
}





//void AGameModeCPP::MovePlayer(int CurrentHole)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Moving Player..."));
//
//	//APlayerController* PC = GEngine->GetWorld()->GetFirstPlayerController();
//	//if (PC != nullptr){
//	//	PC->GetPawn()->SetActorLocation(FVector(5860.f, -8449.f, 555.f));
//	//	APlayerPawn::SetActorLocation();
//
//	//}
//	//FActorSpawnParameters spawnParams;
//	//FRotator rotator = FRotator(0, 0, r[CurrentHole]);
//	//FVector spawnLocation = FVector(x[CurrentHole], y[CurrentHole], z[CurrentHole]); //default to top left 
//
//	////GetWorld()->SpawnActor<APawn>(PlayerPawn, spawnLocation, rotator, spawnParams);
//}



void AGameModeCPP::MazeGenerationBegin()
{ 

	//Depth first here
	DepthFirstMaze(mazeSize);




	//FActorSpawnParameters spawnParams;
	//FRotator rotator = FRotator(0,0,0); //this is where you add rotation to the pieces
	//FVector spawnLocation = FVector(0.f,0.f,0.f); //spawn at 0,0,0

	//GetWorld()->SpawnActor<AActor>(MazeC, spawnLocation, rotator, spawnParams);

	//rotator = FRotator(0, 180, 0); 
	//spawnLocation = FVector(-2000.f, 0.f, 0.f); 
	//GetWorld()->SpawnActor<AActor>(MazeC, spawnLocation, rotator, spawnParams);

	//rotator = FRotator(0, 0, 0);
	//spawnLocation = FVector(0.f, 0.f, 1.f);
	//GetWorld()->SpawnActor<AActor>(MazeFloor, spawnLocation, rotator, spawnParams);

}

int ** AGameModeCPP::DepthFirstMaze(int size)
{
	/*
	Concept:
		Create sizexsize matrix to represent the grid
			Size must be an odd number for this to work properly, 
			otherwise the some sides might be 2 walls thick

		Fill every cell in the grid with "0" (a wall)
			0 is used since c++ sets vectors to 0 by default 
				-> makes resetting much faster & easier

		Start somewhere random across an edge
			Note this position as where the playerstart will be later

		Move around matrix 2 steps at a time to create a maze
			Do not move if out of bounds or hitting a path
			If its an okay spot to move to, move and set position to "1"

		Note final location as where the flag will be
		Output this grid and start and ending positions
	*/
	
	//assume size = 9
	//the grid is always a square (easier for this and not really neccessary to be rectangle here)
	//although rectangles could be used to make wide short levels.
	//could modify this to add those later.

	vector<vector<int>> grid(size, vector<int>(size)); //create the grid of sizexsize

	int startX = FMath::RandRange(0, size); //later these will be public variables for playerstart.
	int starty = FMath::RandRange(0, size);

	int curX = startX;
	int curY = starty;

	// 0 = North , 1 = East , 2 = South, 3 = West
	int lookDir = FMath::RandRange(0, 3);

	// 0 = wall
	// 1 = path

	/*
	Might generate something like this:
	starting location is 3,1
	ending location is 4,4
	maze = 	0	0	0	0	0
			0	1	1	0	0
			0	1	0	0	0
			0	1	0	1	0	
			0	1	1	1	0
	*/

	switch (lookDir){
		case 0: //North
		{
			if (curY <= 1) { //out of north bounds
				break;
			}
			else if (grid[curX][curY-2] == 0) { //see if north block 2 away is a wall
				curY = curY - 2; //if so, move there
				grid[curX][curY] = 1; //set that location to be a path
				break;
			}
			break; //should only be reached if that location is a path
		}
		case 1: //East
		{
			if (curX >= size-1) { //out of east bounds [..] [size-1] [size]| -> out of bounds
				break;
			}
			else if (grid[curX+2][curY] == 0) { //see if east block 2 away is a wall
				curX = curX + 2; //if so, move there
				grid[curX][curY] = 1; //set that location to be a path
				break;
			}
			break; //should only be reached if that location is a path
		}
		case 2: //South
		{										//      [..]
			if (curY >= size-1) { //out of south bounds [size-1] 
				break;							//		[size] 
			}									//      \/ out of bounds
			else if (grid[curX][curY + 2] == 0) { //see if south block 2 away is a wall
				curY = curY + 2; //if so, move there
				grid[curX][curY] = 1; //set that location to be a path
				break;
			}
			break; //should only be reached if that location is a path
		}
		case 3: //West
		{
			if (curX <= 1) { //out of west bounds
				break;
			}
			else if (grid[curX - 2][curY] == 0) { //see if west block 2 away is a wall
				curX = curX - 2; //if so, move there
				grid[curX][curY] = 1; //set that location to be a path
				break;
			}
			break; //should only be reached if that location is a path
		}
	}



	//set playerstart to whatever
	//set flag to whatever
	return nullptr; //return the grid
	
}


