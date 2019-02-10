// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGeneration.h"

#include "Engine.h"
#include "Engine/World.h"
#include "UnrealMathUtility.h"
#include <iostream>
#include <algorithm>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */



using namespace std;
int MazeGeneration::generateMaze(int r, int c)
{
	if (DrawDebugText)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Recursive generateMaze function called")));


	if (deadEndHit && noDeadEndsAllowed) {
		return 0;
	}
	PrintMaze();
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
				endX = r - 2;
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
				endY = c + 2;
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
				endX = r + 2;
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
				endY = c - 2;
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

void MazeGeneration::PrintMaze()
{
	for (int i = 0; i < mazeSize; i++) {
		for (int j = 0; j < mazeSize; j++) {
			cout << maze[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void MazeGeneration::depthFirstMaze()
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
	PrintMaze();
}

void MazeGeneration::initMazeGen()
{
}

void MazeGeneration::oldDepthFirstMaze(int size)
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

	//std::vector<std::vector<int>> newGrid(size, vector<int>(size)); //create the grid of sizexsize

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
//void MazeGeneration::oldSnakeToUnreal()
//{
//	FActorSpawnParameters spawnParams;
//	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	FRotator rotator = FRotator(0, 0, 0);
//	FVector spawnLocation = FVector(0.f, 0.f, 0.f);
//	float realX = 0.f;
//	float realY = 0.f;
//	AActor* pieceToAdd;
//
//	for (int i = 0; i < crdList.size(); i++) {
//		realX = crdList[i].x * 2000;
//		realY = crdList[i].y * 2000;
//		spawnLocation = FVector(realX, realY, 0.f);
//
//		if (i == 0) { //first will be a C facing up
//
//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeC, spawnLocation, FRotator(0, 270, 0), spawnParams);
//			AllMazePieces.Add(pieceToAdd);
//		}
//		else if (i == crdList.size() - 1) { //last will be a C facing backwards
//			switch (crdList[i].d) {
//			case -1: //enters west
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeC, spawnLocation, FRotator(0, 0, 0), spawnParams);
//				break;
//			case 0: //enters north
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeC, spawnLocation, FRotator(0, 90, 0), spawnParams);
//				break;
//			case 1: //enters east
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeC, spawnLocation, FRotator(0, 180, 0), spawnParams);
//				break;
//			}
//			AllMazePieces.Add(pieceToAdd);
//		}
//		else {
//			//switch (crdList[i].d) {
//			//case -1: //exits west
//			//	switch (crdList[i - 1].d) {
//			//		case -1: //enters west
//			//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams); // --
//			//			break;
//			//		case 0: //enters north
//			//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 90, 0), spawnParams); // ¬
//			//			break;
//			//		//case 1: //enters east
//			//		//	pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams); // wont happen
//			//		//	break;
//			//		}
//			//	break;
//			//case 0: //exits north
//			//	switch (crdList[i - 1].d) {
//			//		case -1: //enters west
//			//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 90, 0), spawnParams); // -`
//			//			break;
//			//		case 0: //enters north
//			//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams); // |
//			//			break;
//			//		case 1: //enters east
//			//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 90, 0), spawnParams); // `-
//			//			break;
//			//		}
//			//	break;
//			//case 1: //exits east
//			//	switch (crdList[i - 1].d) {
//			//		//case -1: //enters west
//			//		//	pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams); // -- wont happen
//			//		//	break;
//			//		case 0: //enters north
//			//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 270, 0), spawnParams); // ,-
//			//			break;
//			//		case 1: //enters east
//			//			pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams); // --
//			//			break;
//			//		}
//			//	break;
//			//}
//			if (crdList[i].d == 0 && crdList[i + 1].d == -1) { //enter south , exit west
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 180, 0), spawnParams);
//			}
//			else if (crdList[i].d == 0 && crdList[i + 1].d == 0) { //enter/exit north/sout
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams);
//			}
//			else if (crdList[i].d == 0 && crdList[i + 1].d == 1) { //enter south , exit east
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 90, 0), spawnParams);
//			}
//
//
//
//			else if (crdList[i].d == 1 && crdList[i + 1].d == 0) { //enter west , exit north
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 270, 0), spawnParams);
//			}
//
//			else if (crdList[i].d == 1 && crdList[i + 1].d == 1) { //enter/exit west/east
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams);
//			}
//
//			else if (crdList[i].d == -1 && crdList[i + 1].d == 0) { //enter east , exit north
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 0, 0), spawnParams);
//			}
//			else if (crdList[i].d == -1 && crdList[i + 1].d == -1) { //enter east , exit east
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams);
//			}
//
//			else {
//				pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeX, spawnLocation, FRotator(0, 0, 0), spawnParams);
//			}
//
//
//			AllMazePieces.Add(pieceToAdd);
//
//		}
//
//
//
//
//		//pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeX, spawnLocation, rotator, spawnParams);
//		//AllMazePieces.Add(pieceToAdd);
//	}
//
//	spawnLocation = FVector(realX, realY, 0.f);
//	pieceToAdd = GetWorld()->SpawnActor<AActor>(FlagBP, spawnLocation, rotator, spawnParams);
//	AllMazePieces.Add(pieceToAdd);
//
//}

