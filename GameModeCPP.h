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
	bool DrawDebugText = true;
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
	void printMaze();
	void depthFirstMaze();
	void DFMtoUnreal();
	int row, col;

	void placePiece(int x, int y);
	TSubclassOf<class AActor> pieceToPlace = MazeN; //by default place a solid block
	int pieceToRotate = 0; //rotation is 0 by default
	void resetDFM();
	bool DiceRoll(int percentage);
	
	void GenerateSnakeMaze();
	TArray<FVector> usedLocations;
	int globalRotation = 0;
	int localRotation = 0;
	int trackLength = 10;
};

