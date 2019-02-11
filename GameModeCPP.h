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
	
	virtual void InitGameState();


	//
	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class APawn> PlayerPawn;

	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class AController> PlayerController
	

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

	//Maze Gen
	UPROPERTY(EditAnywhere, Category = Maze , meta = (ClampMin = "5", UIMin = "5"))
		int mazeWidth;
	UPROPERTY(EditAnywhere, Category = Maze, meta = (ClampMin = "5", UIMin = "5"))
		int mazeHeight;
	UPROPERTY(EditAnywhere, Category = Maze, meta = (ClampMin = "1", UIMin = "1"))
		int pathLength;
	UPROPERTY(EditAnywhere, Category = Maze)
		bool noDeadEndsAllowed;

	//Cave Gen
	UPROPERTY(EditAnywhere, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
		int maxCaveX;
	UPROPERTY(EditAnywhere, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
		int maxCaveY;
	UPROPERTY(EditAnywhere, Category = Cave, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int createChance;
	UPROPERTY(EditAnywhere, Category = Cave)
		int maxCyclesInitial;
	UPROPERTY(EditAnywhere, Category = Cave)
		int maxCyclesFinal;
	UPROPERTY(EditAnywhere, Category = Cave)
		float minSizeMultiplier;

public:


	// Sets default values for this actors's properties
	AGameModeCPP();

	void MazeGenerationBegin(); //add in things like "nodeadends" here, etc.
	void SnakeGenerationBegin();

	std::vector<std::vector<int>> maze;

	bool DrawDebugText = true; //--------------------------------------------------------------------debug text

	TArray<AActor*> AllMazePieces;

	TArray<TSubclassOf<AActor>> MazePieces;
	TArray<TSubclassOf<AActor>> MazePiecesAlt1;

	std::vector<std::vector<int>> grid;

	void DFMtoUnreal();
	int row, col;
	void placePiece(int x, int y);
	TSubclassOf<class AActor> pieceToPlace = MazeN; //by default place a solid block
	int pieceToRotate = 0; //rotation is 0 by default
	void resetMap();
	bool DiceRoll(int percentage);
	int pieceType;

	//TArray<FVector> usedLocations;
	//FVector currentLocation = FVector(0, 0, 0);
	//int globalRotation = 0;
	//int localRotation = 0;
	//int trackLength = 3;




	//--------SNAKE GEN---------\\

	int curX; //current x
	int curY; //current y
	int trackLength; //total amount of spaces to check
	crd checkCrd; //coordinate we want to move to
	std::vector<crd> crdList; //vector to hold coordinates

	//search through crdlist for a crd
	bool Exists(crd toFind , std::vector<crd> listToCheck);
	void SnakeToUnreal();
	UPROPERTY(EditAnywhere, Category = Snake)
		int snakeTrackLength; //0-100 chance to spawn a room of some kind


	//--------ROOM GEN---------\\

	int direction; //0 1 2 3 = N E S W | Previous Direction
	std::vector<crd> roomList; //vector to hold coordinates of the rooms
	int minX, minY, maxX, maxY; //to guage how big the map is
	int wallsPlaced = 0; //ditto walls
	int roomsPlaced = 0; //just to keep track of how many rooms are placed
	//Modifiers:
	UPROPERTY(EditAnywhere, Category = RoomGen)
		int roomChance; //0-100 chance to spawn a room of some kind
	UPROPERTY(EditAnywhere, Category = RoomGen)
		int turnChance; //0-100 chance to turn vs go in a straight line
	UPROPERTY(EditAnywhere, Category = RoomGen)
		bool hitSelf = true; //true = path can go through itself 
	UPROPERTY(EditAnywhere, Category = RoomGen)
		bool turnBack = true; //true = path can turn backwards (requires hitSelf = true)
	UPROPERTY(EditAnywhere, Category = RoomGen)
		int roomPathLength;
	UPROPERTY(EditAnywhere, Category = RoomGen , meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int chanceForRock;//0 for no rocks, 100 for every space to be a rock.
	UPROPERTY(EditAnywhere, Category = RoomGen)
		bool placeCornerWalls;//0 for no rocks, 100 for every space to be a rock.

	//search through crdlist for a crd
	int HardCodedRotate(int randomDirection);
	void MoveBorders();
	void roomGeneration();
	void MakeRoom3x3(crd newCoord);
	int Rotate(int randomDirection);
	void generateWalls();
	void roomToUnreal();
	bool directionOkay = true; //by default the direction being moved to is safe
	crd flagLocation = crd{ 0,0,0 };

	AActor* placeRock(float x, float y); //Places a rock at a location then randomly rotates, scales and positions.
	int startX, startY;

	std::vector<std::vector<node>> caveMap;
	void caveToUnreal();
	CaveGeneration caveGen;

	MazeGeneration mazeGen;
	SnakeGeneration snakeGen;
	RoomGeneration roomGen;

};