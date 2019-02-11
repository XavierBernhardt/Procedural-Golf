// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "vector"
#include "GameFramework/GameMode.h"

struct crd {
	int x, y; //x, y = coordinates
	int d;	//	d = direction , -1 = left , 0 = up , 1 = right
	//d can be inferred from the previous entry in the array, however it's just easier to store it as a value.
	// - > it takes little space to store it vs setting up a method to deduce where it came from

	//------ ROOM GEN ------
	//0 = floor , 1 = wall
	//Different to snake gen in that the generator uses 'tank controls'
	//-> it turns or moves forwards/ back, rather than moving in an absolute direction.
	//d here is used to decide if it is a wall or floor, rather than direction.
};
/**
 * 
 */
class PROCEDURALGOLFV2_API SnakeGeneration
{
public:
	
	int curX; //current x
	int curY; //current y
	int trackLength; //total amount of spaces to check
	crd checkCrd; //coordinate we want to move to
	std::vector<crd> crdList; //vector to hold coordinates
	bool DrawDebugText = true;

	//search through crdlist for a crd
	bool Exists(crd toFind, std::vector<crd> listToCheck);
	void GenerateSnakeMaze();
	std::vector<crd> initSnakeGen(int _trackLength);

	
};
