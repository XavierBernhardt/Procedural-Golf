// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "vector"
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



public:


	// Sets default values for this actors's properties
	AGameModeCPP();

	void MazeGenerationBegin(); //add in things like "nodeadends" here, etc.
	void SnakeGenerationBegin();


	static const int mazeSize = 5;
	static const int pathLength = 9;
	float startX;
	float startY;
	float endX;
	float endY;
	bool DrawDebugText = false;
	TArray<AActor*> AllMazePieces;

	TArray<TSubclassOf<AActor>> MazePieces;
	TArray<TSubclassOf<AActor>> MazePiecesAlt1;

	//static int mazeSize = 5;
	int maze[mazeSize][mazeSize];
	//int pathLength;
	bool deadEndHit = false;

	UPROPERTY(EditAnywhere)
	bool noDeadEndsAllowed;

	//vector<vector<int>> DepthFirstMaze(int size);
	void oldDepthFirstMaze(int size);

	std::vector<std::vector<int>> grid;

	//static void MovePlayer(int CurrentHole);

	//int CurrentHole; //current hole

	//float x[8]; //x of spawn
	//float y[8]; //y of spawn
	//float z[8]; //z of spawn
	//float r[8]; //yaw rotation of spawn
	int generateMaze(int r, int c);
	void PrintMaze();
	void depthFirstMaze();
	void DFMtoUnreal();
	int row, col;

	void placePiece(int x, int y);
	TSubclassOf<class AActor> pieceToPlace = MazeN; //by default place a solid block
	int pieceToRotate = 0; //rotation is 0 by default
	void resetMap();
	bool DiceRoll(int percentage);
	int pieceType;
	void GenerateSnakeMaze();

	//TArray<FVector> usedLocations;
	//FVector currentLocation = FVector(0, 0, 0);
	//int globalRotation = 0;
	//int localRotation = 0;
	//int trackLength = 3;


	//coordinate struct
	struct crd {
		int x, y; //x, y = coordinates
		int d;	//	d = direction , -1 = left , 0 = up , 1 = right
		//d can be inferred from the previous entry in the array, however it's just easier to store it as a value.
		// - > it takes little space to store it vs setting up a method to deduce where it came from

		//could create real map back to front using the vector as a stack?
		/*
			ignore below
		bool n, e, s, w; //walls
		crd() :	x(0), //set to 0,0 by default
				y(0),
				n(true), //give it solid walls by default
				e(true),
				s(true),
				w(true) {}
		*/
	};

	//initiate variables
	int curX; //current x
	int curY; //current y
	int trackLength; //total amount of spaces to check
	crd checkCrd; //coordinate we want to move to
	std::vector<crd> crdList; //vector to hold coordinates

	//search through crdlist for a crd
	bool Exists(crd toFind);
	void SnakeToUnreal();


};

