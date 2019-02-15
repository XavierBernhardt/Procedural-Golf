// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "vector"
#include "CaveGeneration.h"
#include "MazeGeneration.h"
#include "SnakeGeneration.h"
#include "RoomGeneration.h"

#include "GameModeCPP.generated.h"

UCLASS()
class PROCEDURALGOLFV2_API AGameModeCPP : public AGameMode
{
	GENERATED_BODY()

protected:

	//Variables
	bool DrawDebugText = true;
	TSubclassOf<class AActor> pieceToPlace = MazeN; //by default place a solid block
	int pieceToRotate = 0; //rotation is 0 by default
	int pieceType;
	AActor* placeRock(float x, float y); //Places a rock at a location then randomly rotates, scales and positions.
	int chanceForRock;

	//Arrays/Vectors
	std::vector<crd> roomList; //vector to hold coordinates of the rooms
	std::vector<std::vector<node>> caveMap;
	TArray<AActor*> allMazePieces;
	TArray<TSubclassOf<AActor>> mazePieces;
	TArray<TSubclassOf<AActor>> mazePiecesAlt1;
	//std::vector<std::vector<int>> maze;
	std::vector<crd> crdList; //vector to hold coordinates

	//Generation instances
	CaveGeneration caveGen;
	MazeGeneration mazeGen;
	SnakeGeneration snakeGen;
	RoomGeneration roomGen;

	//Methods
	void mazeGenBegin(); //add in things like "nodeadends" here, etc.
	void snakeGenBegin();
	bool diceRoll(int percentage);
	bool exists(crd toFind , std::vector<crd> listToCheck);
	void placePiece(int x, int y);
	void mazeToUnreal();
	void caveToUnreal();
	void SnakeToUnreal();
	void roomToUnreal();
	virtual void InitGameState();
	void roomGenBegin();
	void caveGenBegin();

public:

	int startX, startY; //Ball starting location (default x = 0 , y = 0)
	void resetMap();
	int currentMap; //0 Menu | 1 maze | 2 snake | 3 room | 4 cave | 5 natural

protected:
	////--------////--------Object References---------\\\\---------\\\\

	UPROPERTY(EditAnywhere, Category = Main)
		TSubclassOf<class AActor> PlayerPawn;

	//Maze 1
	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeNodeMain;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeN;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeC;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeL;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeI;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeT;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeX;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeFloor;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> FlagBP;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> FlagNoBase;

	//Maze 2 (Curved)
	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeNAlt1;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeCAlt1;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeLAlt1;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeIAlt1;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeTAlt1;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeXAlt1;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt1N;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt1C;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt1L;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt1I;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt1T;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt1X;

	//Rocks
	UPROPERTY(EditAnywhere, Category = Rocks)
		TSubclassOf<class AActor> Rock1;

	UPROPERTY(EditAnywhere, Category = Rocks)
		TSubclassOf<class AActor> Rock2;

	UPROPERTY(EditAnywhere, Category = Rocks)
		TSubclassOf<class AActor> Rock3;
};



//Old code

	//bool directionOkay = true; //by default the direction being moved to is safe
	//crd flagLocation = crd{ 0,0,0 };
	////search through crdlist for a crd
	////int HardCodedRotate(int randomDirection);
	////void MoveBorders();
	////void roomGeneration();
	////void MakeRoom3x3(crd newCoord);
	////int Rotate(int randomDirection);
	////void generateWalls();
	//int minX, minY, maxX, maxY; //to guage how big the map is
	//int wallsPlaced = 0; //ditto walls
	//int roomsPlaced = 0; //just to keep track of how many rooms are placed
	//int curX; //current x
	//int curY; //current y
	//int trackLength; //total amount of spaces to check
	//crd checkCrd; //coordinate we want to move to
	////TArray<FVector> usedLocations;
	////FVector currentLocation = FVector(0, 0, 0);
	////int globalRotation = 0;
	////int localRotation = 0;
	////int trackLength = 3;
	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class APawn> PlayerPawn;
	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class AController> PlayerController
	//int direction; //0 1 2 3 = N E S W | Previous Direction
	//std::vector<std::vector<int>> grid;	
	//int row, col;

	//Before setting up the main menu settings window, I used these values as editAnywheres to modify the values within the editor


	////--------////--------MODIFIERS---------\\\\---------\\\\

	////--------MAZE GEN---------\\\\

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "5", UIMin = "5"))
	//	int mazeWidth;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "5", UIMin = "5"))
	//	int mazeHeight;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "1", UIMin = "1"))
	//	int pathLength;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
	//	bool noDeadEndsAllowed;

	////--------SNAKE GEN---------\\\\

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Snake)
	//	int snakeTrackLength; //0-100 chance to spawn a room of some kind

	//////--------ROOM GEN---------\\\\

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
	//	int roomChance; //0-100 chance to spawn a room of some kind
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
	//	int turnChance; //0-100 chance to turn vs go in a straight line
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
	//	bool hitSelf = true; //true = path can go through itself 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
	//	bool turnBack = true; //true = path can turn backwards (requires hitSelf = true)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
	//	int roomPathLength;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	//	int chanceForRock;//0 for no rocks, 100 for every space to be a rock.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
	//	bool placeCornerWalls;//0 for no rocks, 100 for every space to be a rock.

	//////--------CAVE GEN---------\\\\

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
	//	int maxCaveX;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
	//	int maxCaveY;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	//	int createChance;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
	//	int maxCyclesInitial;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
	//	int maxCyclesFinal;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
	//	float minSizeMultiplier;