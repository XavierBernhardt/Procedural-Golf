// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeCPP.h"
#include "Engine.h"
#include "Engine/World.h"
#include "PlayerPawn.h"
#include "UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"
#include <iostream>
#include <algorithm>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */

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
		if (DrawDebugText)
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("InitState"));

		FString LevelName = GetWorld()->GetMapName();
		LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

//		GEngine->AddOnScreenDebugMessage(-1, 99.0f, FColor::White, FString::Printf(TEXT("Level=%s"), *LevelName));


		if (LevelName.Equals("MazeGeneration")) {
			if (DrawDebugText)
			GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Maze Generation"));
			MazeGenerationBegin();
		}
		else if (LevelName.Equals("ControlMap")) {
			if (DrawDebugText)
			GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Control Map"));
			
		}
		if (LevelName.Equals("SnakeGeneration")) {
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Snake Generation"));
			SnakeGenerationBegin();
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
	//oldDepthFirstMaze(mazeSize);
	
	//0 = C, 1 = I, 2 = L , 3 = N , 4 = T , 5 = X 
	
	MazePieces.Add(MazeC);
	MazePieces.Add(MazeI);
	MazePieces.Add(MazeL);
	MazePieces.Add(MazeN);
	MazePieces.Add(MazeT);
	MazePieces.Add(MazeX);

	MazePiecesAlt1.Add(MazeCAlt1);
	MazePiecesAlt1.Add(MazeIAlt1);
	MazePiecesAlt1.Add(MazeLAlt1);
	MazePiecesAlt1.Add(MazeNAlt1);
	MazePiecesAlt1.Add(MazeTAlt1);
	MazePiecesAlt1.Add(MazeXAlt1);

	MazePiecesAlt1;

	depthFirstMaze();
	if (DrawDebugText) {
		for (int i = 0; i < 5; i++) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("%i %i %i %i %i"), maze[i][0], maze[i][1], maze[i][2], maze[i][3], maze[i][4]));
		}
	}

	DFMtoUnreal();
}

void AGameModeCPP::SnakeGenerationBegin()
{
	MazePieces.Add(MazeC);
	MazePieces.Add(MazeI);
	MazePieces.Add(MazeL);
	MazePieces.Add(MazeN);
	MazePieces.Add(MazeT);
	MazePieces.Add(MazeX);

	MazePiecesAlt1.Add(MazeCAlt1);
	MazePiecesAlt1.Add(MazeIAlt1);
	MazePiecesAlt1.Add(MazeLAlt1);
	MazePiecesAlt1.Add(MazeNAlt1);
	MazePiecesAlt1.Add(MazeTAlt1);
	MazePiecesAlt1.Add(MazeXAlt1);

	GenerateSnakeMaze();
}
void AGameModeCPP::GenerateSnakeMaze()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	float realX = 0.f;
	float realY = 0.f;
	FRotator rotator = FRotator(0, 0, 0); 
	FVector spawnLocation = FVector(realX, realY, 0.f);

	AActor* pieceToAdd;


	if (DiceRoll(ChanceForAlt1C))
		pieceToPlace = MazePiecesAlt1[0]; //1 open wall = C piece
	else
		pieceToPlace = MazePieces[0]; //1 open wall = C piece

	pieceToAdd = GetWorld()->SpawnActor<AActor>(pieceToPlace, spawnLocation, rotator, spawnParams);
	AllMazePieces.Add(pieceToAdd);


	realY = realY + 2000;

	spawnLocation = FVector(realX, realY, 0.f);

	if (DiceRoll(ChanceForAlt1C))
		pieceToPlace = MazePiecesAlt1[0]; //1 open wall = C piece
	else
		pieceToPlace = MazePieces[0]; //1 open wall = C piece

	pieceToAdd = GetWorld()->SpawnActor<AActor>(pieceToPlace, spawnLocation, FRotator(0, 180, 0), spawnParams);
	AllMazePieces.Add(pieceToAdd);

}

//vector<vector<int>> AGameModeCPP::DepthFirstMaze(int size


int AGameModeCPP::generateMaze(int r, int c)
{
	if (DrawDebugText)
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Recursive generateMaze function called")));


	if (deadEndHit && noDeadEndsAllowed) {
		return 0;
	}
	printMaze();
	// 4 random directions
	int randDirs[4];
	cout << "\n generateMaze called \n";
	for (int i = 0; i < 4; i++) {
		randDirs[i] = i;
	}
	std::random_shuffle(&randDirs[0], &randDirs[4]);
	cout << "Random direction order is: " << randDirs[0] << randDirs[1] << randDirs[2] << randDirs[3] << "\n";

	// Examine each direction
	for (int i = 0; i < 4; i++) {
		switch (randDirs[i]) {
		case 0: // Up
			cout << "Checking up \n";
			//　Whether 2 cells up is out or not
			if (r - 2 < 0)
				continue;
			if (maze[r - 2][c] != 0) {
				maze[r - 2][c] = 0;
				maze[r - 1][c] = 0;
				endX = r-2;
				endY = c;
				generateMaze(r - 2, c);
			}
			break;
		case 1: // Right
			cout << "Checking right \n";
			// Whether 2 cells to the right is out or not
			if (c + 2 >= mazeSize)
				continue;
			if (maze[r][c + 2] != 0) {
				maze[r][c + 2] = 0;
				maze[r][c + 1] = 0;
				endX = r;
				endY = c+2;
				generateMaze(r, c + 2);
			}
			break;
		case 2: // Down
			cout << "Checking down \n";
			// Whether 2 cells down is out or not
			if (r + 2 >= mazeSize)
				continue;
			if (maze[r + 2][c] != 0) {
				maze[r + 2][c] = 0;
				maze[r + 1][c] = 0;
				endX = r+2;
				endY = c;
				generateMaze(r + 2, c);
			}
			break;
		case 3: // Left
			cout << "Checking left \n";
			// Whether 2 cells to the left is out or not
			if (c - 2 < 0)
				continue;
			if (maze[r][c - 2] != 0) {
				maze[r][c - 2] = 0;
				maze[r][c - 1] = 0;
				endX = r;
				endY = c-2;
				generateMaze(r, c - 2);
			}
		}
		if (noDeadEndsAllowed) {
			cout << "Hit a dead end!!! \n";
			deadEndHit = true;
			return 0;
		}

	}
	//if (noDeadEndsAllowed) {
	//	cout << "Hit a dead end \n";
	//	deadEndHit = true;
	//	return 0;
	//}
	return 0;
}

void AGameModeCPP::printMaze()
{
	for (int i = 0; i < mazeSize; i++) {
		for (int j = 0; j < mazeSize; j++) {
			cout << maze[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void AGameModeCPP::depthFirstMaze()
{
	if (DrawDebugText)
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("**DepthFirstMaze Begin**")));
	cout << "Begin \n";
	for (int i = 0; i < mazeSize; i++) {
		for (int j = 0; j < mazeSize; j++) {
			maze[i][j] = 1;
		}

	}


	srand(time(NULL));

	//generate a random place along the edge


	if (!noDeadEndsAllowed) {
		int whatside = rand() % 4;

		switch (whatside) {
		case 0:
			row = 0;
			col = rand() % mazeSize;
			break;
		case 1:
			row = mazeSize - 1;
			col = rand() % mazeSize;
			break;
		case 2:
			row = rand() % mazeSize;
			col = 0;
			break;
		case 3:
			row = rand() % mazeSize;
			col = mazeSize - 1;
			break;
		}
	}
	else {
		row = mazeSize / 2;
		col = mazeSize / 2;
	}



	if ((row % 2 != 0) && (row != 0)) {
		row--;
	}
	if ((col % 2 != 0) && (col != 0)) {
		col--;
	}

	maze[row][col] = 0;
	startX = row * 2000;
	startY = col * 2000;
	if (DrawDebugText)
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Gamemode: StartX = %i  StartY = %i"), startX, startY));

	generateMaze(row, col);
	printMaze();
}

void AGameModeCPP::DFMtoUnreal()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
	FRotator rotator = FRotator(0, 0, 0); //this is where you add rotation to the pieces
	FVector spawnLocation = FVector(0.f, 0.f, 0.f); //spawn at 0,0,0
	float realX = 0.f;
	float realY = 0.f;
	AActor* pieceToAdd;

	//2000 x 2000 is the real size of the grids
	//AllMazePieces
		
	//0 = C, 1 = I, 2 = L , 3 = N , 4 = T , 5 = X 
	//MazePieces.Add(MazeC);

	for (int i = 0; i < mazeSize; i++) {
		for (int j = 0; j < mazeSize; j++) {
			cout << maze[i][j] << " ";
			spawnLocation = FVector(realX, realY, 0.f); //spawn at 0,0,0

			if (maze[i][j] == 1) {

				if (DiceRoll(ChanceForAlt1N))
					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazePiecesAlt1[3], spawnLocation, rotator, spawnParams);
				else
					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazePieces[3], spawnLocation, rotator, spawnParams);
					
				AllMazePieces.Add(pieceToAdd);
			}
			else {
				placePiece(i, j);
				rotator = FRotator(0, pieceToRotate, 0);
				pieceToAdd =  GetWorld()->SpawnActor<AActor>(pieceToPlace, spawnLocation, rotator, spawnParams);
				AllMazePieces.Add(pieceToAdd);
			}
			rotator = FRotator(0, 0, 0);
			if (i == endX && j == endY) {
				spawnLocation = FVector(realX, realY, 5.f);
				pieceToAdd = GetWorld()->SpawnActor<AActor>(FlagBP, spawnLocation, rotator, spawnParams);
				AllMazePieces.Add(pieceToAdd);
			}

			realY = realY + 2000;
		}
		realY = 0.f;
		realX = realX + 2000;
	}
	rotator = FRotator(0, 0, 0);
	spawnLocation = FVector(4000.f, 4000.f, 5.f);
	pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeFloor, spawnLocation, rotator, spawnParams);
	AllMazePieces.Add(pieceToAdd);
}

void AGameModeCPP::placePiece(int x, int y)
{
	pieceToPlace = MazeN; //by default place a solid block
	pieceToRotate = 0; //rotation is 0 by default

	//Each wall is open by default
	bool north = false;
	bool east = false;
	bool south = false;
	bool west = false;
	int openWalls = 4; //all 4 are open

	if ((y == 0 )| (maze[x][y-1] == 1)) { //if top row, or there is a wall above
		north = true; //close north wall
		openWalls--;
	}
	if ((x == mazeSize-1) | (maze[x + 1][y] == 1)) { //if top row, or there is a wall above
		east = true; //close east wall
		openWalls--;
	}
	if ((y == mazeSize - 1) | (maze[x][y + 1] == 1)) { //if top row, or there is a wall above
		south = true; //close south wall
		openWalls--;
	}
	if ((x == 0) | (maze[x-1][y] == 1)) { //if top row, or there is a wall above
		west = true; //close west wall
		openWalls--;
	}

	//could use these to generate walls and not place pieces 
	//am going to place pieces however as it allows for more design variation (different L pieces etc)
	
	switch (openWalls) { 	//0 = C, 1 = I, 2 = L , 3 = N , 4 = T , 5 = X 
		case (0): { // no open walls
			pieceToPlace = MazePiecesAlt1[3]; //place filled block piece
			break;
		}
		case (1): { // 1 open wall 

			if (DiceRoll(ChanceForAlt1C))
				pieceToPlace = MazePiecesAlt1[0]; //1 open wall = C piece
			else
				pieceToPlace = MazePieces[0]; //1 open wall = C piece
			if (west == false) pieceToRotate = 0;
			else if (north == false) pieceToRotate = 90;
			else if (east == false) pieceToRotate = 180;
			else pieceToRotate = 270;
			break;
		}
		case (2): { // 2 open walls

			if ((north == true && south == true) || (west == true && east == true)) { 
				if (DiceRoll(ChanceForAlt1I))
					pieceToPlace = MazePiecesAlt1[1]; 
				else
					pieceToPlace = MazePieces[1]; 			
				if (west == true) pieceToRotate = 90;
			}
			else { //bottom and right are open by default rotation
				if (DiceRoll(ChanceForAlt1L))
					pieceToPlace = MazePiecesAlt1[2]; 
				else
					pieceToPlace = MazePieces[2];

				if (north == true && east == true) pieceToRotate = 0;
				else if (east == true && south == true) pieceToRotate = 90;
				else if (south == true && west == true) pieceToRotate = 180;
				else pieceToRotate = 270;
			}
			break;
		}
		case (3): { // 3 open walls
			if (DiceRoll(ChanceForAlt1T))
				pieceToPlace = MazePiecesAlt1[4]; 
			else
				pieceToPlace = MazePieces[4]; 
											  //left wall closed
			if (north == true) pieceToRotate = 0; 
			else if (east == true) pieceToRotate = 90; 
			else if (south == true) pieceToRotate = 180;
			else pieceToRotate = 270;

			break;
		}
		case (4): { // 4 open walls
			if (DiceRoll(ChanceForAlt1X))
				pieceToPlace = MazePiecesAlt1[5];
			else
				pieceToPlace = MazePieces[5]; 
			break;
		}
	}
	

}

void AGameModeCPP::resetDFM()
{
	for (int i = 0; i != AllMazePieces.Num(); ++i)
	{
		AllMazePieces[i]->Destroy();
	}
	AllMazePieces.Reset();
	MazeGenerationBegin();
}

bool AGameModeCPP::DiceRoll(int percentage)
{
	if (percentage == 0) {
		return false;
	}
	return (FMath::RandRange(1, 100 / percentage) == 1 ? true : false);
}




void AGameModeCPP::oldDepthFirstMaze(int size)
{
	///*
	//Concept:
	//	Create sizexsize matrix to represent the grid
	//		Size must be an odd number for this to work properly, 
	//		otherwise the some sides might be 2 walls thick

	//	Fill every cell in the grid with "0" (a wall)
	//		0 is used since c++ sets vectors to 0 by default 
	//			-> makes resetting much faster & easier

	//	Start somewhere random across an edge
	//		Note this position as where the playerstart will be later

	//	Move around matrix 2 steps at a time to create a maze
	//		Do not move if out of bounds or hitting a path
	//		If its an okay spot to move to, move and set position to "1"

	//	Note final location as where the flag will be
	//	Output this grid and start and ending positions
	//*/
	//
	////assume size = 9
	////the grid is always a square (easier for this and not really neccessary to be rectangle here)
	////although rectangles could be used to make wide short levels.
	////could modify this to add those later.

	//vector<vector<int>> newGrid(size, vector<int>(size)); //create the grid of sizexsize

	//startX = FMath::RandRange(0, size); //later these will be public variables for playerstart.
	//startY = FMath::RandRange(0, size);

	//int curX = startX;
	//int curY = startY;

	//int pathToGo = pathLength;

	//// 0 = wall
	//// 1 = path

	///*
	//Might generate something like this:
	//starting location is 3,1
	//ending location is 4,4
	//maze = 	0	0	0	0	0
	//		0	1	1	0	0
	//		0	1	0	0	0
	//		0	1	0	1	0	
	//		0	1	1	1	0
	//*/


	//while (pathToGo >= 0) {

	//	//Select a random direction to go in
	//	//0 = North , 1 = East , 2 = South, 3 = West
	//	int lookDir = FMath::RandRange(0, 3);

	//	switch (lookDir) {
	//	case 0: //North
	//	{
	//		if (curY <= 1) { //out of north bounds
	//			break;
	//		}
	//		else if (grid[curX][curY - 2] == 0) { //see if north block 2 away is a wall
	//			curY = curY - 2; //if so, move there
	//			grid[curX][curY] = 1; //set that location to be a path
	//			pathToGo--;
	//			break;
	//		}
	//		break; //should only be reached if that location is a path
	//	}
	//	case 1: //East
	//	{
	//		if (curX >= size - 1) { //out of east bounds [..] [size-1] [size]| -> out of bounds
	//			break;
	//		}
	//		else if (grid[curX + 2][curY] == 0) { //see if east block 2 away is a wall
	//			curX = curX + 2; //if so, move there
	//			grid[curX][curY] = 1; //set that location to be a path
	//			pathToGo--;
	//			break;
	//		}
	//		break; //should only be reached if that location is a path
	//	}
	//	case 2: //South
	//	{										//      [..]
	//		if (curY >= size - 1) { //out of south bounds [size-1] 
	//			break;							//		[size] 
	//		}									//      \/ out of bounds
	//		else if (grid[curX][curY + 2] == 0) { //see if south block 2 away is a wall
	//			curY = curY + 2; //if so, move there
	//			grid[curX][curY] = 1; //set that location to be a path
	//			pathToGo--;
	//			break;
	//		}
	//		break; //should only be reached if that location is a path
	//	}
	//	case 3: //West
	//	{
	//		if (curX <= 1) { //out of west bounds
	//			break;
	//		}
	//		else if (grid[curX - 2][curY] == 0) { //see if west block 2 away is a wall
	//			curX = curX - 2; //if so, move there
	//			grid[curX][curY] = 1; //set that location to be a path
	//			pathToGo--;
	//			break;
	//		}
	//		break; //should only be reached if that location is a path
	//	}
	//	}
	//}


	////Grid
	//grid = newGrid;

	////Flag coordinates
	//endX = curX;
	//endY = curY;

	////Player coordinates are startX and startY

	////return grid; //return the grid
}