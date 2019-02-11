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

	int mazeWidth; //Must be odd
	int mazeHeight; //Must be odd

	int pathLength;
	float startX, startY, endX, endY;
	bool DrawDebugText = true; 
	bool deadEndHit = false;
	int row, col;
	bool noDeadEndsAllowed;
	
	TArray<AActor*> AllMazePieces;
	std::vector<std::vector<int>> maze;
	
	std::vector<std::vector<int>> initMazeGen(int _mazeWidth, int _mazeHeight, int _pathLength, bool _noDeadEndsAllowed);
	void depthFirstMaze();
	int recursiveMaze(int r, int c);
	void PrintMaze();
	void oldDepthFirstMaze(int size);

};
