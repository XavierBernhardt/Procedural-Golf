// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "vector"
#include "GameFramework/GameMode.h"
/**
 * 
 */
class PROCEDURALGOLFV2_API MazeGeneration
{
public:

	static const int mazeSize = 5;
	static const int pathLength = 9;
	float startX, startY, endX, endY;
	bool DrawDebugText = true; 
	int maze[mazeSize][mazeSize];
	bool deadEndHit = false;
	int row, col;
	bool noDeadEndsAllowed;
	
	TArray<AActor*> AllMazePieces;
	std::vector<std::vector<int>> grid;

	void initMazeGen();
	void depthFirstMaze();
	int recursiveMaze(int r, int c);
	void PrintMaze();
	void oldDepthFirstMaze(int size);

};
