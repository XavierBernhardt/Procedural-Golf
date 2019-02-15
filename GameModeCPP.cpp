// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeCPP.h"
#include "Engine.h"
#include "Engine/World.h"
#include "PlayerPawn.h"
#include "UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"
#include "MazeNodeMain.h"
#include "RockActor.h"
#include <iostream>
#include <algorithm>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */
#include "GameInstanceCPP.h"

using namespace std;

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
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		GEngine->AddOnScreenDebugMessage(-1, 99.0f, FColor::White, FString::Printf(TEXT("Level=%s"), *LevelName));
		currentMap = 0;
		if (LevelName.Equals("MainMenu")) {
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Main Menu"));
		//	mazeGenBegin();

		}

		else if (LevelName.Equals("MazeGeneration")) {
			currentMap = 1;
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Maze Generation"));
			mazeGenBegin();
			GetWorld()->SpawnActor<AActor>(PlayerPawn, FVector(startX, startY,0.f), FRotator(0, 0, 0), spawnParams);
		}
		else if (LevelName.Equals("ControlMap")) {
			currentMap = 100;
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Control Map"));
			GetWorld()->SpawnActor<AActor>(PlayerPawn, FVector(0.f, 0.f, 0.f), FRotator(0, 0, 0), spawnParams);
		}
		else if (LevelName.Equals("SnakeGeneration")) {
			currentMap = 2;
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Snake Generation"));
			snakeGenBegin();
			GetWorld()->SpawnActor<AActor>(PlayerPawn, FVector(0.f, 0.f, 0.f), FRotator(0, 0, 0), spawnParams);
		}
		else if (LevelName.Equals("RoomGeneration")) {
			currentMap = 3;
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Room Generation"));
			roomGenBegin();

			GetWorld()->SpawnActor<AActor>(PlayerPawn, FVector(0.f, 0.f, 0.f), FRotator(0, 0, 0), spawnParams);
		}
		else if (LevelName.Equals("CaveGeneration")) {
			currentMap = 4;
			if (DrawDebugText)
				GEngine->AddOnScreenDebugMessage(110, 99.f, FColor::Cyan, TEXT("Current Level: Cave Generation"));


			GetWorld()->SpawnActor<AActor>(PlayerPawn, FVector(startX, startY, 0.f), FRotator(0, 0, 0), spawnParams);
		}


		/*
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
		
		
		*/

	}
}

void AGameModeCPP::roomGenBegin()
{

	roomSettings roomSet = { 10, 30, true, true, 30, 20, true }; //Default settings

	if (currentMap == 0) { //If on the menu
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Currently on menu")));
	}
	else {
		UGameInstanceCPP * gameInst = Cast<UGameInstanceCPP>(GetWorld()->GetGameInstance());
		if (gameInst) { //Try to get the custom values set in the main menu 
			roomSet = gameInst->getRoomSettings();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Game instance found")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Could not find game instance")));
		}

	}


	crdList = roomGen.initRoomGen(roomSet.roomChance, roomSet.turnChance, roomSet.hitSelf, 
			roomSet.turnBack, roomSet.pathLength, roomSet.rockChance, roomSet.placeCorners);

	roomToUnreal();
}

void AGameModeCPP::caveGenBegin()
{

	caveSettings caveSet = { 25, 25, 40, 3, 3, 0.3 };
	if (currentMap == 0) { //If on the menu
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Currently on menu")));
	}
	else {
		UGameInstanceCPP * gameInst = Cast<UGameInstanceCPP>(GetWorld()->GetGameInstance());
		if (gameInst) { //Try to get the custom values set in the main menu 
			caveSet = gameInst->getCaveSettings();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Game instance found")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Could not find game instance")));
		}

	}


	caveMap = caveGen.initCaveGen(caveSet.maxCaveX, caveSet.maxCaveY, caveSet.createChance, 
		caveSet.initialCycles, caveSet.finalCycles, caveSet.minSizeMulti);

	caveToUnreal();
}

void AGameModeCPP::mazeGenBegin()
{ 
	//0 = C, 1 = I, 2 = L , 3 = N , 4 = T , 5 = X 
	
	mazePieces.Add(MazeC);
	mazePieces.Add(MazeI);
	mazePieces.Add(MazeL);
	mazePieces.Add(MazeN);
	mazePieces.Add(MazeT);
	mazePieces.Add(MazeX);

	mazePiecesAlt1.Add(MazeCAlt1);
	mazePiecesAlt1.Add(MazeIAlt1);
	mazePiecesAlt1.Add(MazeLAlt1);
	mazePiecesAlt1.Add(MazeNAlt1);
	mazePiecesAlt1.Add(MazeTAlt1);
	mazePiecesAlt1.Add(MazeXAlt1);


	mazeSettings mazeSet = { 5,5,0,false }; //Default settings

	if (currentMap == 0) { //If on the menu
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Currently on menu")));	
	}
	else { 
		UGameInstanceCPP * gameInst = Cast<UGameInstanceCPP>(GetWorld()->GetGameInstance());
		if (gameInst) { //Try to get the custom values set in the main menu 
			mazeSet = gameInst->getMazeSettings();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Game instance found")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Could not find game instance")));
		}

	}
	
	mazeGen.initMazeGen(mazeSet.width, mazeSet.height, mazeSet.length, mazeSet.deadends);
	mazeToUnreal();

	startX = mazeGen.startX;
	startY = mazeGen.startY;
}

void AGameModeCPP::snakeGenBegin()
{
	if (DrawDebugText)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Snake Gen Begin")));

	mazePieces.Add(MazeC);
	mazePieces.Add(MazeI);
	mazePieces.Add(MazeL);
	mazePieces.Add(MazeN);
	mazePieces.Add(MazeT);
	mazePieces.Add(MazeX);

	mazePiecesAlt1.Add(MazeCAlt1);
	mazePiecesAlt1.Add(MazeIAlt1);
	mazePiecesAlt1.Add(MazeLAlt1);
	mazePiecesAlt1.Add(MazeNAlt1);
	mazePiecesAlt1.Add(MazeTAlt1);
	mazePiecesAlt1.Add(MazeXAlt1);

	int snakeLength = 20;

	UGameInstanceCPP * gameInst = Cast<UGameInstanceCPP>(GetWorld()->GetGameInstance());
	if (gameInst) { //Try to get the custom values set in the main menu 
		snakeLength = gameInst->GIsnakeTrackLength;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Game instance found")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Could not find game instance")));
	}

	crdList = snakeGen.initSnakeGen(snakeLength);
	SnakeToUnreal();

}

void AGameModeCPP::mazeToUnreal()
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

	for (int i = 0; i < mazeGen.mazeWidth; i++) {
		for (int j = 0; j < mazeGen.mazeHeight; j++) {
			cout << mazeGen.maze[i][j] << " ";
			spawnLocation = FVector(realX, realY, 0.f); //spawn at 0,0,0

			if (mazeGen.maze[i][j] == 1) {
				AMazeNodeMain* mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, rotator, spawnParams);
				mazeNode->setType(3);
				mazeNode->setFloor(0);
				mazeNode->init();
				allMazePieces.Add(mazeNode);
			}
			else {
				placePiece(i, j);
				rotator = FRotator(0, pieceToRotate, 0);
				AMazeNodeMain* mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, rotator, spawnParams);
				mazeNode->setType(pieceType);
				mazeNode->setFloor(0);
				mazeNode->init();
				allMazePieces.Add(mazeNode);

			}
			rotator = FRotator(0, 0, 0);
			if (i == mazeGen.endX && j == mazeGen.endY) {
				spawnLocation = FVector(realX, realY, 5.f);
				pieceToAdd = GetWorld()->SpawnActor<AActor>(FlagBP, spawnLocation, rotator, spawnParams);
				allMazePieces.Add(pieceToAdd);
			}

			realY = realY + 2000;
		}
		realY = 0.f;
		realX = realX + 2000;
	}
	//rotator = FRotator(0, 0, 0);
	//spawnLocation = FVector(4000.f, 4000.f, 5.f);
	//pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeFloor, spawnLocation, rotator, spawnParams);
	//AllMazePieces.Add(pieceToAdd);
}

void AGameModeCPP::SnakeToUnreal()
{
	//0 1 2 3 4 5
	//C I L N T X
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator rotator = FRotator(0, 0, 0);
	FVector spawnLocation = FVector(0.f, 0.f, 0.f);
	float realX = 0.f;
	float realY = 0.f;
	AActor* pieceToAdd;
	AMazeNodeMain* 	mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
	mazeNode->Destroy(); //got to initialise it for it to compile

	for (int i = 0; i < crdList.size(); i++) {
		realX = crdList[i].x * 2000;
		realY = crdList[i].y * 2000;
		spawnLocation = FVector(realX, realY, 0.f);

		if (i == 0) { //first will be a C facing up
			mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 270, 0), spawnParams);
			mazeNode->setType(0);
			mazeNode->setFloor(0);
			mazeNode->init();
			allMazePieces.Add(mazeNode);
		}
		else if (i == crdList.size() - 1) { //last will be a C facing backwards
			switch (crdList[i].d) {
			case -1: //enters west
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
				mazeNode->setType(0);
				mazeNode->setFloor(0); //set to 0 for hole (experemental)
				mazeNode->init();
				break;
			case 0: //enters north
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 90, 0), spawnParams);
				mazeNode->setType(0);
				mazeNode->setFloor(0);
				mazeNode->init();
				break;
			case 1: //enters east
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 180, 0), spawnParams);
				mazeNode->setType(0);
				mazeNode->setFloor(0);
				mazeNode->init();
				break;
			}
			allMazePieces.Add(mazeNode);
		}
		else {
			/*
			switch (crdList[i].d) {
			case -1: //exits west
				switch (crdList[i - 1].d) {
					case -1: //enters west
						pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams); // --
						break;
					case 0: //enters north
						pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 90, 0), spawnParams); // ¬
						break;
					//case 1: //enters east
					//	pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams); // wont happen
					//	break;
					}
				break;
			case 0: //exits north
				switch (crdList[i - 1].d) {
					case -1: //enters west
						pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 90, 0), spawnParams); // -`
						break;
					case 0: //enters north
						pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams); // |
						break;
					case 1: //enters east
						pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 90, 0), spawnParams); // `-
						break;
					}
				break;
			case 1: //exits east
				switch (crdList[i - 1].d) {
					//case -1: //enters west
					//	pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams); // -- wont happen
					//	break;
					case 0: //enters north
						pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 270, 0), spawnParams); // ,-
						break;
					case 1: //enters east
						pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 0, 0), spawnParams); // --
						break;
					}
				break;
			}*/
			if (crdList[i].d == 0 && crdList[i + 1].d == -1) { //enter south , exit west
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 180, 0), spawnParams);
				mazeNode->setType(2);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			else if (crdList[i].d == 0 && crdList[i + 1].d == 0) { //enter/exit north/sout
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 90, 0), spawnParams);
				mazeNode->setType(1);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			else if (crdList[i].d == 0 && crdList[i + 1].d == 1) { //enter south , exit east
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 90, 0), spawnParams);
				mazeNode->setType(2);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			else if (crdList[i].d == 1 && crdList[i + 1].d == 0) { //enter west , exit north
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 270, 0), spawnParams);
				mazeNode->setType(2);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			else if (crdList[i].d == 1 && crdList[i + 1].d == 1) { //enter/exit west/east
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
				mazeNode->setType(1);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			else if (crdList[i].d == -1 && crdList[i + 1].d == 0) { //enter east , exit north
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
				mazeNode->setType(2);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			else if (crdList[i].d == -1 && crdList[i + 1].d == -1) { //enter east , exit east
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
				mazeNode->setType(1);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			else {
				mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
				mazeNode->setType(5);
				mazeNode->setFloor(0);
				mazeNode->init();
			}
			allMazePieces.Add(mazeNode);
		}
	}
	spawnLocation = FVector(realX, realY, 0.f);
	pieceToAdd = GetWorld()->SpawnActor<AActor>(FlagBP, spawnLocation, rotator, spawnParams); //FlagNoBase for hole

	allMazePieces.Add(pieceToAdd);
}

void AGameModeCPP::roomToUnreal()
{
	//0 1 2 3 4 5 6
	//C I L N T X F
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator rotator = FRotator(0, 0, 0);
	FVector spawnLocation = FVector(0.f, 0.f, 0.f);
	float realX = 0.f;
	float realY = 0.f;
	AActor* pieceToAdd;
	//AMazeNodeMain* 	mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
	//mazeNode->Destroy(); //got to initialise it for it to compile

	for (int i = 0; i < crdList.size(); i++) {

		realX = crdList[i].x * 2000;
		realY = crdList[i].y * 2000;
		spawnLocation = FVector(realX, realY, 0.f);

		//roomList here is used to store a list of all coordinates used, to prevent duplicates.
		//Duplicate checking was moved here so it only runs once per coordinate, as opposed to for every possible check each coordinate makes.

		if (!exists(crdList[i], roomList)) //avoid placing duplicate nodes with only one check per loop
		{
			AMazeNodeMain* mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);
			if (crdList[i].d == 1) { //If this is a wall
				mazeNode->setType(6); //sand wall
				mazeNode->setFloor(-1); //no floor
			}
			else { //If this is a floor
				mazeNode->setType(-1); //no wall
				mazeNode->setFloor(4); //sand floor
			}
			mazeNode->init();
			allMazePieces.Add(mazeNode);
			roomList.emplace_back(crdList[i]);
		}

		if (crdList[i].x == roomGen.flagLocation.x && crdList[i].y == roomGen.flagLocation.y) { //If on the flag coordinates
			pieceToAdd = GetWorld()->SpawnActor<AActor>(FlagBP, spawnLocation, rotator, spawnParams); //FlagNoBase for hole
			allMazePieces.Add(pieceToAdd); //Add the flag
		}
		else { //If not on the flag coordinates
			if ((crdList[i].d != 1) && (i != 0)) { //If not on a wall & not on the player spawn
				if (diceRoll(chanceForRock)) { //Maybe place a rock
					allMazePieces.Add(placeRock(realX, realY)); //place a rock & add it to the array
				}
			}
		}
	}
	//Relinquish memory 
	roomList.clear(); //empty the list of placed objects 
	crdList.clear(); //empty the list of coordinates
}

void AGameModeCPP::caveToUnreal()
{


	//0 1 2 3 4 5 6
	//C I L N T X F
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator rotator = FRotator(0, 0, 0);
	FVector spawnLocation = FVector(0.f, 0.f, 0.f);
	float realX = 0.f;
	float realY = 0.f;
	AActor* pieceToAdd;

	for (auto x = 0; x < caveMap.size(); x++) { //Loop through horizontal
		for (auto y = 0; y < caveMap[0].size(); y++) { //Loop through vertical
			realX = x * 1000; //Cave floors/walls are half the size of the other maps
			realY = y * 1000;
			spawnLocation = FVector(realX, realY, 0.f);
			AMazeNodeMain* mazeNode = GetWorld()->SpawnActor<AMazeNodeMain>(MazeNodeMain, spawnLocation, FRotator(0, 0, 0), spawnParams);

			if (caveMap[x][y].type == 1) {  //If this is a wall
				mazeNode->setType(7); //Cave wall
				mazeNode->setFloor(-1); //no floor
			}
			else { //Type: 0, 2, 3 are all floors (floor / floor + player start / floor + flag end)
				mazeNode->setType(-1); //no wall
				mazeNode->setFloor(3); //Cave floor
			}

			mazeNode->init();
			allMazePieces.Add(mazeNode);

			if (caveMap[x][y].type == 3) { //Flag
				pieceToAdd = GetWorld()->SpawnActor<AActor>(FlagBP, spawnLocation, rotator, spawnParams); //FlagNoBase for hole
				allMazePieces.Add(pieceToAdd); //Add the flag
			}
			else if (caveMap[x][y].type == 2) { //Player Spawn
				startX = realX;
				startY = realY;
			}
		}
	}
}

void AGameModeCPP::placePiece(int x, int y)
{
	pieceToPlace = MazeN; //by default place a solid block
	pieceToRotate = 0; //rotation is 0 by default
	pieceType = 0;
	//Each wall is open by default
	bool north = false;
	bool east = false;
	bool south = false;
	bool west = false;
	int openWalls = 4; //all 4 are open

	if ((y == 0 )|| (mazeGen.maze[x][y-1] == 1)) { //if top row, or there is a wall above
		north = true; //close north wall
		openWalls--;
	}
	if ((x == mazeGen.maze.size()-1) || (mazeGen.maze[x + 1][y] == 1)) { //if top row, or there is a wall above
		east = true; //close east wall
		openWalls--;
	}
	if ((y == mazeGen.maze.at(x).size() - 1) || (mazeGen.maze[x][y + 1] == 1)) { //if top row, or there is a wall above
		south = true; //close south wall
		openWalls--;
	}
	if ((x == 0) || (mazeGen.maze[x-1][y] == 1)) { //if top row, or there is a wall above
		west = true; //close west wall
		openWalls--;
	}

	//could use these to generate walls and not place pieces 
	//am going to place pieces however as it allows for more design variation (different L pieces etc)
	
	switch (openWalls) { 	//0 = C, 1 = I, 2 = L , 3 = N , 4 = T , 5 = X 
		case (0): { // no open walls
			pieceType = 3;
			break;
		}
		case (1): { // 1 open wall 

			pieceType = 0;
			if (west == false) pieceToRotate = 0;
			else if (north == false) pieceToRotate = 90;
			else if (east == false) pieceToRotate = 180;
			else pieceToRotate = 270;
			break;
		}
		case (2): { // 2 open walls

			if ((north == true && south == true) || (west == true && east == true)) { 
				pieceType = 1;
				if (west == true) pieceToRotate = 90;
			}
			else { //bottom and right are open by default rotation
				pieceType = 2;

				if (north == true && east == true) pieceToRotate = 0;
				else if (east == true && south == true) pieceToRotate = 90;
				else if (south == true && west == true) pieceToRotate = 180;
				else pieceToRotate = 270;
			}
			break;
		}
		case (3): { // 3 open walls
			pieceType = 4;	  
			if (north == true) pieceToRotate = 0; 
			else if (east == true) pieceToRotate = 90; 
			else if (south == true) pieceToRotate = 180;
			else pieceToRotate = 270;

			break;
		}
		case (4): { // 4 open walls
			pieceType = 5; //could probably do pieceType = openWalls-1;
			break;
		}
	}
	

}

bool AGameModeCPP::exists(crd toFind, std::vector<crd> listToCheck)
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

bool AGameModeCPP::diceRoll(int percentage)
{
	if (percentage == 0) {
		return false;
	}
	return (FMath::RandRange(1, 100 / percentage) == 1 ? true : false);
}

void AGameModeCPP::resetMap()
{
	AGameState* MyGameState = Cast<AGameState>(GameState);
	if (MyGameState)
	{
		FString LevelName = GetWorld()->GetMapName();
		LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);


		for (int i = 0; i != allMazePieces.Num(); ++i)
		{
			//if (AllMazePieces[i]!=nullptr)
			allMazePieces[i]->Destroy();
		}
		allMazePieces.Reset();

		if (LevelName.Equals("MazeGeneration")) {
			mazeGenBegin();
		}
		else if (LevelName.Equals("ControlMap")) {
		}
		else if (LevelName.Equals("SnakeGeneration")) {
			snakeGenBegin();
		}
		else if (LevelName.Equals("RoomGeneration")) {
			roomGenBegin();
		}
		else if (LevelName.Equals("CaveGeneration")) {
			caveGenBegin();
		}
	}
}

AActor* AGameModeCPP::placeRock(float x, float y)
{
	FActorSpawnParameters spawnParams;	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //force it to always spawn
	FRotator randomRotation = FRotator(0, float(FMath::RandRange(0, 359)), 0); //Pick a random rotation to use
	FVector randomHeight = FVector(x, y, float(FMath::RandRange(-30, 0))); //Randomly sink it more into the ground

	int roll = FMath::RandRange(1, 3); //Pick a number 1 to 3

	switch (roll) { //decide which other rock will be used
	case 1:
		return GetWorld()->SpawnActor<AActor>(Rock1, randomHeight, randomRotation, spawnParams); //return the rock placed
		break;
	case 2:
		return GetWorld()->SpawnActor<AActor>(Rock2, randomHeight, randomRotation, spawnParams); //return the rock placed
		break;
	default:
		return GetWorld()->SpawnActor<AActor>(Rock3, randomHeight, randomRotation, spawnParams); //return the rock placed
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
//

//void AGameModeCPP::GenerateSnakeMazeOLD()
//{
//	if (DrawDebugText)
//		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Generating Snake Maze")));
//
//	FActorSpawnParameters spawnParams;
//	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	float realX = 0.f;
//	float realY = 0.f;
//	FRotator rotator = FRotator(0, 270, 0); 
//	FVector spawnLocation = FVector(realX, realY, 0.f);
//
//	AActor* pieceToAdd;
//
//
//	if (DiceRoll(ChanceForAlt1C))
//		pieceToPlace = MazePiecesAlt1[0];
//	else
//		pieceToPlace = MazePieces[0];
//
//	pieceToAdd = GetWorld()->SpawnActor<AActor>(pieceToPlace, spawnLocation, rotator, spawnParams);
//	AllMazePieces.Add(pieceToAdd);
//	usedLocations.Add(currentLocation);
//
//	realY = realY + 2000;
//	spawnLocation = FVector(realX, realY, 0.f);
//	pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams);
//	AllMazePieces.Add(pieceToAdd);
//
//
//	//if (DiceRoll(ChanceForAlt1C))
//	//	pieceToPlace = MazePiecesAlt1[0]; //1 open wall = C piece
//	//else
//	//	pieceToPlace = MazePieces[0]; //1 open wall = C piece
//
//
//
//
//	for (int i = 0; i < trackLength; i++) {
//		if (DrawDebugText)
//			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Loop Start")));
//
//		int randDirs[3] = { -1, 0, 1 }; //left , straight , right
//		std::random_shuffle(&randDirs[0], &randDirs[3]); //randomise the order they're checked in
//
//		if (DrawDebugText)
//			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Random order is: %i %i %i"), randDirs[0], randDirs[1], randDirs[2]));
//
//
//		FVector checkLocation = FVector(realX, realY, 0.f);
//
//		for (int i = 0; i < 3; i++) {
//			switch (randDirs[i]) {
//			case -1: // Left
//
//				if (DrawDebugText)
//					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Checking LEFT")));
//				checkLocation = FVector(realX+2000, realY, 0.f);
//				if (usedLocations.Find(checkLocation) == INDEX_NONE) {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location safe, moving left...")));
//					realX = realX + 2000;
//					spawnLocation = FVector(realX, realY, 0.f);
//
//					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 180, 0), spawnParams);
//					AllMazePieces.Add(pieceToAdd);
//					usedLocations.Add(spawnLocation);
//					break;
//				}
//				else {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location bad (left)")));
//					continue;
//				}
//				break;
//
//			case 0: // Straight
//
//				if (DrawDebugText)
//					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Checking STRAIGHT")));
//				checkLocation = FVector(realX, realY + 2000, 0.f);
//				if (usedLocations.Find(checkLocation) == INDEX_NONE) {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location safe, moving straight...")));
//					realY = realY + 2000;
//					spawnLocation = FVector(realX, realY, 0.f);
//					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams);
//					AllMazePieces.Add(pieceToAdd);
//					usedLocations.Add(spawnLocation);
//					break;
//				}
//				else {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location bad (straight)")));
//					continue;
//				}
//				break;
//
//			case 1: // Right
//
//				if (DrawDebugText)
//					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Checking RIGHT")));
//				checkLocation = FVector(realX - 2000, realY, 0.f);
//				if (usedLocations.Find(checkLocation) == INDEX_NONE) {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location safe, moving right...")));
//					realX = realX - 2000;
//					spawnLocation = FVector(realX, realY, 0.f);
//
//					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 180, 0), spawnParams);
//					AllMazePieces.Add(pieceToAdd);
//					usedLocations.Add(spawnLocation);
//					break;
//				}
//				else {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location bad (right)")));
//					continue;
//				}
//				break;
//
//
//			}
//		}
//
//
//	}

//void AGameModeCPP::GenerateSnakeMazeOld2()
//{
//	if (DrawDebugText)
//		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Generating Snake Maze")));
//
//	FActorSpawnParameters spawnParams;
//	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	float realX = 0.f;
//	float realY = 0.f;
//	FRotator rotator = FRotator(0, 270, 0);
//	FVector spawnLocation = FVector(realX, realY, 0.f);
//
//	AActor* pieceToAdd;
//
//
//	if (DiceRoll(ChanceForAlt1C))
//		pieceToPlace = MazePiecesAlt1[0];
//	else
//		pieceToPlace = MazePieces[0];
//
//	pieceToAdd = GetWorld()->SpawnActor<AActor>(pieceToPlace, spawnLocation, rotator, spawnParams);
//	AllMazePieces.Add(pieceToAdd);
//	usedLocations.Add(currentLocation);
//
//	realY = realY + 2000;
//	spawnLocation = FVector(realX, realY, 0.f);
//	pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams);
//	AllMazePieces.Add(pieceToAdd);
//
//
//	//if (DiceRoll(ChanceForAlt1C))
//	//	pieceToPlace = MazePiecesAlt1[0]; //1 open wall = C piece
//	//else
//	//	pieceToPlace = MazePieces[0]; //1 open wall = C piece
//
//
//
//
//	for (int i = 0; i < trackLength; i++) {
//		if (DrawDebugText)
//			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Loop Start")));
//
//		int randDirs[3] = { -1, 0, 1 }; //left , straight , right
//		std::random_shuffle(&randDirs[0], &randDirs[3]); //randomise the order they're checked in
//
//		if (DrawDebugText)
//			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Random order is: %i %i %i"), randDirs[0], randDirs[1], randDirs[2]));
//
//
//		FVector checkLocation = FVector(realX, realY, 0.f);
//
//		for (int i = 0; i < 3; i++) {
//			switch (randDirs[i]) {
//			case -1: // Left
//
//				if (DrawDebugText)
//					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Checking LEFT")));
//				checkLocation = FVector(realX + 2000, realY, 0.f);
//				if (usedLocations.Find(checkLocation) == INDEX_NONE) {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location safe, moving left...")));
//					realX = realX + 2000;
//					spawnLocation = FVector(realX, realY, 0.f);
//
//					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 180, 0), spawnParams);
//					AllMazePieces.Add(pieceToAdd);
//					usedLocations.Add(spawnLocation);
//					break;
//				}
//				else {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location bad (left)")));
//					continue;
//				}
//				break;
//
//			case 0: // Straight
//
//				if (DrawDebugText)
//					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Checking STRAIGHT")));
//				checkLocation = FVector(realX, realY + 2000, 0.f);
//				if (usedLocations.Find(checkLocation) == INDEX_NONE) {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location safe, moving straight...")));
//					realY = realY + 2000;
//					spawnLocation = FVector(realX, realY, 0.f);
//					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeI, spawnLocation, FRotator(0, 90, 0), spawnParams);
//					AllMazePieces.Add(pieceToAdd);
//					usedLocations.Add(spawnLocation);
//					break;
//				}
//				else {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location bad (straight)")));
//					continue;
//				}
//				break;
//
//			case 1: // Right
//
//				if (DrawDebugText)
//					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Checking RIGHT")));
//				checkLocation = FVector(realX - 2000, realY, 0.f);
//				if (usedLocations.Find(checkLocation) == INDEX_NONE) {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location safe, moving right...")));
//					realX = realX - 2000;
//					spawnLocation = FVector(realX, realY, 0.f);
//
//					pieceToAdd = GetWorld()->SpawnActor<AActor>(MazeL, spawnLocation, FRotator(0, 180, 0), spawnParams);
//					AllMazePieces.Add(pieceToAdd);
//					usedLocations.Add(spawnLocation);
//					break;
//				}
//				else {
//					if (DrawDebugText)
//						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT("Location bad (right)")));
//					continue;
//				}
//				break;
//
//
//			}
//		}
//
//
//	}
//
//	//realY = realY + 2000;
//	//spawnLocation = FVector(realX, realY, 0.f);
//
//
//
//	//if (DiceRoll(ChanceForAlt1C))
//	//	pieceToPlace = MazePiecesAlt1[0]; //1 open wall = C piece
//	//else
//	//	pieceToPlace = MazePieces[0]; //1 open wall = C piece
//
//	//pieceToAdd = GetWorld()->SpawnActor<AActor>(pieceToPlace, spawnLocation, FRotator(0, 90, 0), spawnParams);
//	//AllMazePieces.Add(pieceToAdd);
//
//}

//int AGameModeCPP::HardCodedRotate(int randomDirection)
//{ //Input Fwd / Bck / CW / CCW  aka 0 2 1 3
//	switch (direction)
//	{
//	case 0: //North
//		if (randomDirection == 0) //forwards
//			direction = 0;
//		else if (randomDirection == 2) //backwards
//			direction = 2;
//		else if (randomDirection == 1) //turn CW
//			direction = 1;
//		else if (randomDirection == -1) //turn CCW
//			direction = 3;
//		break;
//	case 1: //East
//		if (randomDirection == 0) //forwards
//			direction = 1;
//		else if (randomDirection == 2) //backwards
//			direction = 3;
//		else if (randomDirection == 1) //turn CW
//			direction = 2;
//		else if (randomDirection == -1) //turn CCW
//			direction = 0;
//		break;
//	case 2: //South
//		if (randomDirection == 0) //forwards
//			direction = 2;
//		else if (randomDirection == 2) //backwards
//			direction = 0;
//		else if (randomDirection == 1) //turn CW
//			direction = 3;
//		else if (randomDirection == -1) //turn CCW
//			direction = 1;
//		break;
//	case 3: //West
//		if (randomDirection == 0) //forwards
//			direction = 3;
//		else if (randomDirection == 2) //backwards
//			direction = 1;
//		else if (randomDirection == 1) //turn CW
//			direction = 0;
//		else if (randomDirection == -1) //turn CCW
//			direction = 2;
//		break;
//	}
//	return direction; //Absolute direction to move in based on rotation
//}

//void AGameModeCPP::MoveBorders()
//{
//	if (curX < minX) { //if this is the most left we have been
//		minX = curX;
//	}
//	if (curX > maxX) { //if this is the most right we have been
//		maxX = curX;
//	}
//	if (curY < minY) { //if this is the most down we have been
//		minY = curY;
//	}
//	if (curY > maxY) { //if this is the most up we have been
//		maxY = curY;
//	}
//}

//void AGameModeCPP::roomGeneration()
//{
//	//initiate variables
//	curX = 0; //current x
//	curY = 0; //current y
//	direction = 0; //Previous (absolute) direction - Defualt North
//	checkCrd = crd{ 0,0 }; //coordinate we want to move to, start at 0,0
//	srand(time(0)); //seed 
//	//Clear the lists just in case this is not the first run
//	crdList.clear();
//	roomList.clear();
//	std::cout << "hitSelf = " << hitSelf << " turnBack = " << turnBack << " turnChance = " << turnChance << "\n";
//
//	//mark 0,0 and 0,1 as taken (starting area is default)
//	crd newCoord{ curX, curY }; //first coord is always 0,0
//	crdList.emplace_back(newCoord);
//
//	/* (Defaults)
//	roomPathLength = 30; //Must be above 3 or more
//	roomChance = 10; //% of paths will spawn a room
//	turnChance = 50; //% chance to turn vs go in a straight line
//	hitSelf = true; //true = path can go through itself 
//	turnBack = true; //true = path can turn backwards (requires hitSelf = true)
//	*/
//
//
//	//place roomPathLength amount of paths
//	for (int i = 0; i < roomPathLength - 1; i++) { //start placing paths
//		std::cout << "\nStarting loop \n";
//
//		//Create a random list of rotations
//
//		int randRotate[4] = { 0, 1, -1, 2 }; //0 = forwards , 1 = turn CW , -1 = turn CCW , 2 = backwards
//
//		if (turnBack) { randRotate[3] = 0; } //If turning backwards is disabled, prevent that from being an option by making it check f twice.
//
//		if (DiceRoll(turnChance)) { //If there is a chance to turn...
//			std::random_shuffle(&randRotate[0], &randRotate[4]); //randomise the order they're checked in
//			for (int i = 0; i < 4; i++) {
//				if (randRotate[i] == 0) {
//					std::swap(randRotate[i], randRotate[3]); //swap "forwards" to the end, so it is checked last.
//				}
//			}
//		} //Could just randomly decide to go straight too.
//		//Could change by looking for 0 and putting that at the back to ensure a straight line is the last option
//
//		std::cout << "Random order = " << randRotate[0] << randRotate[1] << randRotate[2] << randRotate[3] << "\n";
//		bool endMe = false; //for breaking the loop once a good location has been found
//
//		//Create a list of directions to check, based on the list of rotations
//		int checkDirs[4] = { Rotate(randRotate[0]), Rotate(randRotate[1]), Rotate(randRotate[2]), Rotate(randRotate[3]) }; //Could be simplified but this works fine.
//		std::cout << "Random directions = " << checkDirs[0] << checkDirs[1] << checkDirs[2] << checkDirs[3] << "\n";
//
//		//Check these directions one by one in the random order
//		//As soon as one of these directions is found to be okay, break, move and start this loop again.
//		for (int i = 0; i < 4; i++) { //check the 4 directions
//			switch (checkDirs[i]) {
//			case 0: // North
//				if (endMe) break;
//				directionOkay = true; //by default this direction is safe
//				std::cout << "Checking North \n";
//				if (!hitSelf) { //If it is not allowed to hit itself
//					if (Exists(crd{ curX,curY + 1, 0 }, crdList)) //Search through array if that coordinate is taken)
//					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
//						std::cout << "North impossible \n";
//						directionOkay = false; //this direction is not okay.
//					}
//				}
//				if (directionOkay) {
//					std::cout << "North is okay \n";
//					curY++; //move up
//					direction = checkDirs[i];
//					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
//					//Checking for duplicate coordinates has been removed, and instead, the program only checks for duplicates when placing actors for real
//					//Removing constant checks to prevent creating duplicate coordinates speeds up generation time enourmously.
//						crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
//					endMe = true;
//					break;
//				}
//			case 1: // East
//				if (endMe) break;
//				directionOkay = true; //by default this direction is safe
//				std::cout << "Checking East \n";
//				if (!hitSelf) { //If it is not allowed to hit itself
//					if (Exists(crd{ curX + 1,curY, 0 }, crdList)) //Search through array if that coordinate is taken)
//					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
//						std::cout << "East impossible \n";
//						directionOkay = false; //this direction is not okay.
//					}
//				}
//				if (directionOkay) {
//					std::cout << "East is okay \n";
//					curX++; //move right
//					direction = checkDirs[i];
//					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
//						crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
//					endMe = true;
//					break;
//				}
//			case 2: // South
//				if (endMe) break;
//				directionOkay = true; //by default this direction is safe
//				std::cout << "Checking South \n";
//				if (!hitSelf) { //If it is not allowed to hit itself
//					if (Exists(crd{ curX,curY - 1, 0 }, crdList)) //Search through array if that coordinate is taken)
//					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
//						std::cout << "South impossible \n";
//						directionOkay = false; //this direction is not okay.
//					}
//				}
//				if (directionOkay) {
//					std::cout << "South is okay \n";
//					curY--; //move up
//					direction = checkDirs[i];
//					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
//						crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
//					endMe = true;
//					break;
//				}
//			case 3: // West
//				if (endMe) break;
//				directionOkay = true; //by default this direction is safe
//				std::cout << "Checking West \n";
//				if (!hitSelf) { //If it is not allowed to hit itself
//					if (Exists(crd{ curX - 1,curY, 0 }, crdList)) //Search through array if that coordinate is taken)
//					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
//						std::cout << "West impossible \n";
//						directionOkay = false; //this direction is not okay.
//					}
//				}
//				if (directionOkay) {
//					std::cout << "West is okay \n";
//					curX--; //move left
//					direction = checkDirs[i];
//					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
//						crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
//					endMe = true;
//					break;
//				}
//			}
//		}
//		std::cout << "Floor has been placed. \n";
//
//		MoveBorders(); //Check to see if the current position is out of the border, if so, increase border that way.
//
//		if (DiceRoll(roomChance)) {
//			std::cout << "!!!Placing a room!!! \n";
//
//			//Room list is kept seperate from coord list,
//			//so that the actual "snake" is seperate from the random rooms it drops
//			//so that if hitself = false, it will still be able to move around 
//			// (otherwise it would create a room, surround itself with floors, and be unable to move)
//			//the final product will be both vectors combined to form a big list of floor coordinates
//			//Can create different formations of rooms by adjusting these values, this is just a square.
//			MakeRoom3x3(crd{ curX, curY, 0 });
//			roomsPlaced++;
//		}
//	}
//	flagLocation = crd{ curX, curY, 0 };
//
//	//print out the list
//	for (int i = 0; i < crdList.size(); i++) {
//		std::cout << "crd #" << i
//			<< "     x = " << crdList[i].x
//			<< " y = " << crdList[i].y
//			<< " Wall = " << crdList[i].d
//			<< "\n";
//	}
//
//	crdList.insert(crdList.end(), roomList.begin(), roomList.end()); //concatonate the lists
//	roomList.clear(); //clear that room list to save memory (its not needed now)
//
//
//	//print out the list with rooms included
//	std::cout << "WITH " << roomsPlaced << " ROOMS --------------------\n";
//	for (int i = 0; i < crdList.size(); i++) {
//		std::cout << "crd #" << i
//			<< "     x = " << crdList[i].x
//			<< " y = " << crdList[i].y
//			<< " Wall = " << crdList[i].d
//			<< "\n";
//	}
//
//	generateWalls();
//
//	crdList.insert(crdList.end(), roomList.begin(), roomList.end()); //concatonate the lists
//	roomList.clear(); //clear that room list to save memory (its not needed now)
//	//roomlist was re used to hold the walls.
//
//	//print out the list with walls included
//	std::cout << "WITH " << wallsPlaced << " WALLS --------------------\n";
//	for (int i = 0; i < crdList.size(); i++) {
//		std::cout << "crd #" << i
//			<< "     x = " << crdList[i].x
//			<< " y = " << crdList[i].y
//			<< " Wall = " << crdList[i].d
//			<< "\n";
//	}
//	roomToUnreal(); //Turn this list of walls and floors into a real map
//
//}
//
//void AGameModeCPP::MakeRoom3x3(crd newCoord) //slow ish due to having to check for dupe coords
//{
//	/*
//	0 0	0    x is the current node, 0 is the floors made around it
//	0 x	0
//	0 0	0
//	*/
//
//	//Old method which prevented coordinates having coordinates placed on top of them
//	//Current method has moved checking for duplicates to roomToUnreal, where actors are placed for real
//	//Decided that there's no real reason to care about duplicated coordinates, they take up such a tiny space in memory
//	//	and having to constantly run checks to ensure there are no duplicates was slowing down generation
//	//Generation was already fast, mind you, but moving checks to only be done once at the end for each object in the coordinate list,
//	//	instead of having the checks occur every time a coordinate is checking to maybe be placed, speeds up time astronomically.
//	/*
//			if (!Exists(crd{ curX, curY + 1, 0 }, roomList)		&&		!Exists(crd{ curX, curY + 1, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX, curY + 1, 0 }); //TOP
//			}
//			if (!Exists(crd{ curX + 1, curY + 1, 0 }, roomList) &&		!Exists(crd{ curX + 1, curY + 1, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX + 1, curY + 1, 0 }); //TOP RIGHT
//			}
//			if (!Exists(crd{ curX + 1, curY, 0 }, roomList)		&&		!Exists(crd{ curX + 1, curY, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX + 1, curY, 0 }); //RIGHT
//			}
//			if (!Exists(crd{ curX + 1, curY - 1, 0 }, roomList) &&		!Exists(crd{ curX + 1, curY - 1, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX + 1, curY - 1, 0 }); //BOTTOM RIGHT
//			}
//			if (!Exists(crd{ curX, curY - 1, 0 }, roomList)		&&		!Exists(crd{ curX, curY - 1, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX, curY - 1, 0 }); //BOTTOM
//			}
//			if (!Exists(crd{ curX - 1, curY - 1, 0 }, roomList) &&		!Exists(crd{ curX - 1, curY - 1, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX - 1, curY - 1, 0 }); //BOTTOM LEFT
//			}
//			if (!Exists(crd{ curX - 1, curY, 0 }, roomList)		&&		!Exists(crd{ curX - 1, curY, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX - 1, curY, 0 }); //LEFT
//			}
//			if (!Exists(crd{ curX - 1, curY + 1, 0 }, roomList) &&		!Exists(crd{ curX - 1, curY + 1, 0 }, crdList)) //ensure no duplicate coords
//			{
//				roomList.emplace_back(crd{ curX - 1, curY + 1, 0 }); //TOP LEFT
//			}	
//	*/
//		roomList.emplace_back(crd{ curX, curY + 1, 0 }); //TOP
//		roomList.emplace_back(crd{ curX + 1, curY + 1, 0 }); //TOP RIGHT
//		roomList.emplace_back(crd{ curX + 1, curY, 0 }); //RIGHT
//		roomList.emplace_back(crd{ curX + 1, curY - 1, 0 }); //BOTTOM RIGHT
//		roomList.emplace_back(crd{ curX, curY - 1, 0 }); //BOTTOM
//		roomList.emplace_back(crd{ curX - 1, curY - 1, 0 }); //BOTTOM LEFT	
//		roomList.emplace_back(crd{ curX - 1, curY, 0 }); //LEFT
//		roomList.emplace_back(crd{ curX - 1, curY + 1, 0 }); //TOP LEFT
//}

//int AGameModeCPP::Rotate(int randomDirection)
//{
//	//Input rotation as 0, 2, +1, -1 aka F,B,CW,CCW 
//	//newDirection & direction are absolute world orientations -> 0123 / NESW
//	//Direction is the previously moved direction
//	//New direction is the direction to look at next
//
//   //Try to rotate CW or CCW by adding +1 or -1, might overflow/underflow at 3+1 or 0-1
//   //Forwards changes nothing as F = +0, Backwards is +2 and might overflow.
//	int newDirection = direction + randomDirection;
//
//	//Resolve Overflows & Underflows
//	if (newDirection == -1) newDirection = 3; //If North(0) rotates CCW (-1), loop back to West(3) (only case it will be -1)
//	else if (newDirection == 4) newDirection = 0; //West(3) CW (4) goes North (0), and South(2) Backwards(4) goes North (0)
//	else if (newDirection == 5) newDirection = 1; //West (3) backwards (5) overflows to east (1). (only case it will be 5)
//
//	/* previous method, ignore as have made a shorter version.
//	if (randomDirection == 1 || randomDirection == -1) { //If rotating CW or CCW
//		if (newDirection == 4) newDirection == 0;
//		else if (newDirection == -1) newDirection == 3;
//	}
//	else { //If moving Forwards or Backwards (forwards = +0 , backwards = +2)
//		if (newDirection == 4) newDirection = 0; //If south(2) moves +2(4), loop back to north(0)
//		else if (newDirection == 5) newDirection = 1; //If west(3) moves +2(5), loop back to east(1)
//	}
//	*/
//	return newDirection; //Absolute world direction 0123 / NESW
//}

//void AGameModeCPP::generateWalls()
//{
//
//	//Old method which prevented duplicate walls from being placed
//	/*
//	for (auto i : crdList) {
//
//		//By default, generate a wall on blank space connected on NESW coordinates from the floor.
//		std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";
//
//		//Note:
//		//roomList is used in this method to store walls.
//
//		std::cout << "checking up\n";
//		if (!Exists(crd{ i.x, i.y + 1, 0}, crdList) && !Exists(crd{ i.x, i.y + 1, 1 }, roomList)) { //UP
//			roomList.emplace_back(crd{ i.x, i.y + 1, 1 });
//			std::cout << "placed wall up\n"; wallsPlaced++;
//		}
//
//		std::cout << "checking right\n";
//		if (!Exists(crd{ i.x + 1, i.y, 0 }, crdList) && !Exists(crd{ i.x + 1, i.y, 1 }, roomList)) { //RIGHT
//			roomList.emplace_back(crd{ i.x + 1, i.y, 1 });
//			std::cout << "placed wall right\n"; wallsPlaced++;
//		}
//
//		std::cout << "checking down\n";
//		if (!Exists(crd{ i.x, i.y - 1 , 0}, crdList) && !Exists(crd{ i.x, i.y - 1 , 1 }, roomList)) { //DOWN
//			roomList.emplace_back(crd{ i.x, i.y - 1, 1 });
//			std::cout << "placed wall down\n"; wallsPlaced++;
//		}
//
//		std::cout << "checking left\n";
//		if (!Exists(crd{ i.x - 1, i.y , 0}, crdList) && !Exists(crd{ i.x - 1, i.y , 1 }, roomList)) { //LEFT
//			roomList.emplace_back(crd{ i.x - 1, i.y, 1 });
//			std::cout << "placed wall left\n"; wallsPlaced++;
//		}	
//	}
//	if (placeCornerWalls) { //Seperated from the above loop, so it only checks this if statement once (instead of i:crdlist times)
//		for (auto i : crdList) {
//
//			//Corner walls means NE , SE , SW , NW
//			std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";
//
//			std::cout << "checking up right\n";
//			if (!Exists(crd{ i.x+1, i.y + 1, 0 }, crdList) && !Exists(crd{ i.x+1, i.y + 1, 1 }, roomList)) { //UP RIGHT
//				roomList.emplace_back(crd{ i.x+1, i.y + 1, 1 });
//				std::cout << "placed wall up\n"; wallsPlaced++;
//			}
//
//			std::cout << "checking down right\n";
//			if (!Exists(crd{ i.x + 1, i.y-1, 0 }, crdList) && !Exists(crd{ i.x + 1, i.y - 1, 1 }, roomList)) { //DOWN RIGHT
//				roomList.emplace_back(crd{ i.x + 1, i.y - 1, 1 });
//				std::cout << "placed wall right\n"; wallsPlaced++;
//			}
//
//			std::cout << "checking down left\n";
//			if (!Exists(crd{ i.x - 1, i.y - 1 , 0 }, crdList) && !Exists(crd{ i.x - 1, i.y - 1 , 1 }, roomList)) { //DOWN LEFT
//				roomList.emplace_back(crd{ i.x - 1, i.y - 1, 1 });
//				std::cout << "placed wall down\n"; wallsPlaced++;
//			}
//
//			std::cout << "checking up left\n";
//			if (!Exists(crd{ i.x - 1, i.y+1 , 0 }, crdList) && !Exists(crd{ i.x - 1, i.y + 1 , 1 }, roomList)) { //UP  LEFT
//				roomList.emplace_back(crd{ i.x - 1, i.y + 1, 1 });
//				std::cout << "placed wall left\n"; wallsPlaced++;
//			}
//		}
//	}
//	
//	
//	*/
//	
//
//	//New method does not care about duplicate walls, as explained in the other roomGen methods
//	//   essentially, crds take up a tiny space in memory and it is not worth running tonnes of checks
//	//   constantly to prevent duplicates from occuring
//	//it is far faster to simply check for duplicates once, at the end of generation.
//	//if memory space was a greater concern, the old methods would be preferred, but for this application, allowing duplicates is faster.
//
//	//Of course, walls are not allowed to spawn on top of floors (the point of this method is to create a border of walls around all the floors)
//	//, so one check is still required to occur.
//
//	for (auto i : crdList) {
//		//By default, generate a wall on any blank space connected on NESW coordinates from the floor.
//		std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";
//
//		//Note:
//		//roomList is used in this method to store walls.
//
//		std::cout << "checking up\n";
//		if (!Exists(crd{ i.x, i.y + 1, 0 }, crdList)) { //UP
//			roomList.emplace_back(crd{ i.x, i.y + 1, 1 });
//			std::cout << "placed wall up\n"; wallsPlaced++;
//		}
//
//		std::cout << "checking right\n";
//		if (!Exists(crd{ i.x + 1, i.y, 0 }, crdList)) { //RIGHT
//			roomList.emplace_back(crd{ i.x + 1, i.y, 1 });
//			std::cout << "placed wall right\n"; wallsPlaced++;
//		}
//
//		std::cout << "checking down\n";
//		if (!Exists(crd{ i.x, i.y - 1 , 0 }, crdList)) { //DOWN
//			roomList.emplace_back(crd{ i.x, i.y - 1, 1 });
//			std::cout << "placed wall down\n"; wallsPlaced++;
//		}
//
//		std::cout << "checking left\n";
//		if (!Exists(crd{ i.x - 1, i.y , 0 }, crdList)) { //LEFT
//			roomList.emplace_back(crd{ i.x - 1, i.y, 1 });
//			std::cout << "placed wall left\n"; wallsPlaced++;
//		}
//	}
//	if (placeCornerWalls) { //Seperated from the above loop, so it only checks this if statement once (instead of i:crdlist times)
//		for (auto i : crdList) {
//
//			//Corner walls means NE , SE , SW , NW
//			std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";
//
//			std::cout << "checking up right\n";
//			if (!Exists(crd{ i.x + 1, i.y + 1, 0 }, crdList)) { //UP RIGHT
//				roomList.emplace_back(crd{ i.x + 1, i.y + 1, 1 });
//				std::cout << "placed wall up\n"; wallsPlaced++;
//			}
//
//			std::cout << "checking down right\n";
//			if (!Exists(crd{ i.x + 1, i.y - 1, 0 }, crdList)) { //DOWN RIGHT
//				roomList.emplace_back(crd{ i.x + 1, i.y - 1, 1 });
//				std::cout << "placed wall right\n"; wallsPlaced++;
//			}
//
//			std::cout << "checking down left\n";
//			if (!Exists(crd{ i.x - 1, i.y - 1 , 0 }, crdList)) { //DOWN LEFT
//				roomList.emplace_back(crd{ i.x - 1, i.y - 1, 1 });
//				std::cout << "placed wall down\n"; wallsPlaced++;
//			}
//
//			std::cout << "checking up left\n";
//			if (!Exists(crd{ i.x - 1, i.y + 1 , 0 }, crdList)) { //UP  LEFT
//				roomList.emplace_back(crd{ i.x - 1, i.y + 1, 1 });
//				std::cout << "placed wall left\n"; wallsPlaced++;
//			}
//		}
//	}
//
//
//
//}