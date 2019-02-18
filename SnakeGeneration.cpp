// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakeGeneration.h"
#include "Engine.h"
#include <iostream>
#include <algorithm>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */



void SnakeGeneration::GenerateSnakeMaze()
{
	//initiate variables
	curX = 0; //current x
	curY = 0; //current y
	checkCrd = crd{ 0,0 }; //coordinate we want to move to
	srand(time(0)); //seed 



	//mark 0,0 and 0,1 as taken (starting area and up one are taken by default)
	crd newCoord{ curX, curY }; //first coord is always 0,0
	crdList.emplace_back(newCoord);
	curY++;
	newCoord = crd{ curX, curY }; //move up y by 1 and make a coord there too
	crdList.emplace_back(newCoord);

	//place tracklength amount of tracks
	for (int i = 0; i < trackLength - 2; i++) { //start placing tracks
		if (DrawDebugText)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Starting Loop")));
		int randDirs[3] = { -1, 0, 1 }; //left , straight , right
		std::random_shuffle(&randDirs[0], &randDirs[3]); //randomise the order they're checked in
		if (DrawDebugText)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Random Order = %i , %i , %i"), randDirs[0], randDirs[1], randDirs[2]));

		std::cout << "Random order = " << randDirs[0] << randDirs[1] << randDirs[2] << "\n";
		bool endMe = false;

		for (int i = 0; i < 3; i++) { //check the 3 directions
			std::cout << "Checking directions \n";
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Checking Directions")));
			switch (randDirs[i]) {
			case -1: // Left
				if (endMe) break;
				if (DrawDebugText)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Checking Left")));
				std::cout << "Checking left \n";
				checkCrd = crd{ curX - 1,curY };
				if (Exists(checkCrd, crdList)) //if there's no space to the left
				{
					if (DrawDebugText)
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Left Impossible")));
					std::cout << "Left impossible \n";
					continue; //try another direction
				}
				else {
					if (DrawDebugText)
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Left Okay")));
					std::cout << "Left is okay \n";
					curX--; //move left
					newCoord = crd{ curX, curY , 1 }; //move left 1 and add a new coord there
					crdList.emplace_back(newCoord); //add it to the list
					endMe = true;
					break;
				}

			case 0: // Up
				if (endMe) break;
				if (DrawDebugText)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Checking Up")));
				std::cout << "Checking up \n";
				checkCrd = crd{ curX,curY + 1 };
				if (Exists(checkCrd, crdList)) //if there's no space to the left
				{
					if (DrawDebugText)
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Up Impossible")));
					std::cout << "Up impossible \n";
					continue; //try another direction
				}
				else {
					if (DrawDebugText)
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Up Okay")));
					std::cout << "Up is okay \n";
					curY++; //move up
					newCoord = crd{ curX, curY , 0 }; //move up 1 and add a new coord there
					crdList.emplace_back(newCoord); //add it to the list
					endMe = true;
					break;
				}
			case 1: // Right
				if (endMe) break;
				if (DrawDebugText)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Checking Right")));
				std::cout << "Checking right \n";
				checkCrd = crd{ curX + 1,curY };
				if (Exists(checkCrd, crdList)) //if there's no space to the left
				{
					if (DrawDebugText)
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Right Impossible")));
					std::cout << "Right impossible \n";
					continue; //try another direction
				}
				else {
					if (DrawDebugText)
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Right Okay")));
					std::cout << "Right is okay \n";
					curX++; //move right
					newCoord = crd{ curX, curY, -1 }; //move right 1 and add a new coord there
					crdList.emplace_back(newCoord); //add it to the list
					endMe = true;
					break;
				}
			}
		}
	}

	//print out the list
	//for (int i = 0; i < crdList.size(); i++) {
	//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("crd #%i , x = %i , y = %i , d = %i"), i, crdList[i].x, crdList[i].y, crdList[i].d));

	//	std::cout << "crd #" << i
	//		<< "     x = " << crdList[i].x
	//		<< " y = " << crdList[i].y
	//		<< " d = " << crdList[i].d
	//		<< "\n";
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("\n\n\n\n\n\n")));


}

std::vector<crd> SnakeGeneration::initSnakeGen(int _trackLength)
{
	trackLength = _trackLength;
	crdList.clear();
	GenerateSnakeMaze();
	return crdList;
}



/*				AMazeNodeMain* mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, rotator, spawnParams);
				mazeNode->setType(3);
				mazeNode->setFloor(0);
				mazeNode->init();
				AllMazePieces.Add(mazeNode);*/

bool SnakeGeneration::Exists(crd toFind, std::vector<crd> listToCheck)
{
	std::cout << "Searching array... \n";
	for (int i = 0; i < listToCheck.size(); i++) {
		if ((listToCheck.at(i).x == toFind.x) && (listToCheck.at(i).y == toFind.y)) {
			std::cout << "Coordinate is in use \n";
			return true;
		}
	}
	std::cout << "Coordinate not used \n";
	return false;
}

