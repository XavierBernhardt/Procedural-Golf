// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeGeneration.h"

/**
 * 
 */
class PROCEDURALGOLFV2_API RoomGeneration
{
public:
	int curX; //current x
	int curY; //current y
	crd checkCrd; //coordinate we want to move to
	int direction; //0 1 2 3 = N E S W | Previous Direction
	std::vector<crd> roomList; //vector to hold coordinates of the rooms
	std::vector<crd> crdList; //vector to hold coordinates
	int minX, minY, maxX, maxY; //to guage how big the map is
	int wallsPlaced = 0; //ditto walls
	int roomsPlaced = 0; //just to keep track of how many rooms are placed
	int roomChance; //0-100 chance to spawn a room of some kind
	int turnChance; //0-100 chance to turn vs go in a straight line
	bool hitSelf; //true = path can go through itself 
	bool turnBack; //true = path can turn backwards (requires hitSelf = true)
	int roomPathLength;
	int chanceForRock;//0 for no rocks, 100 for every space to be a rock.
	bool placeCornerWalls;//0 for no rocks, 100 for every space to be a rock.
	int startX, startY;

	int HardCodedRotate(int randomDirection);
	void MoveBorders();
	void roomGeneration();
	void MakeRoom3x3(crd newCoord);
	int Rotate(int randomDirection);
	void generateWalls();
	bool directionOkay = true; //by default the direction being moved to is safe
	crd flagLocation = crd{ 0,0,0 };
	bool Exists(crd toFind, std::vector<crd> listToCheck);
	std::vector<crd> initRoomGen(int _roomChance, int _turnChance, bool _hitSelf, bool _turnBack, 
								int _roomPathLength, int _chanceForRock, bool _placeCornerWalls);
	bool DiceRoll(int percentage);

};
