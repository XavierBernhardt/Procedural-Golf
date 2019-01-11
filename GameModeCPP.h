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

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> MazeN;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> MazeC;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> MazeL;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> MazeI;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> MazeT;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> MazeX;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> MazeFloor;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> FlagBP;

public:


	// Sets default values for this actors's properties
	AGameModeCPP();

	void MazeGenerationBegin(); //add in things like "nodeadends" here, etc.

	static const int mazeSize = 5;
	static const int pathLength = 9;
	float startX;
	float startY;
	float endX;
	float endY;


	//static int mazeSize = 5;
	int maze[mazeSize][mazeSize];
	//int pathLength;
	bool deadEndHit = false;
	bool noDeadEndsAllowed = false;

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


};

