// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomGeneration.h"
#include "Engine.h"
#include "Engine/World.h"
#include "UnrealMathUtility.h"
#include <iostream>
#include <algorithm>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */



std::vector<crd> RoomGeneration::initRoomGen(int _roomChance, int _turnChance, bool _hitSelf, bool _turnBack, int _roomPathLength, int _chanceForRock, bool _placeCornerWalls)
{

	roomChance =_roomChance;
	turnChance = _turnChance;
	hitSelf = _hitSelf;
	turnBack = _turnBack;
	roomPathLength = _roomPathLength;
	chanceForRock = _chanceForRock;
	placeCornerWalls = _placeCornerWalls;

	roomGeneration();

	return crdList;
}



void RoomGeneration::roomGeneration()
{
	//initiate variables
	curX = 0; //current x
	curY = 0; //current y
	direction = 0; //Previous (absolute) direction - Defualt North
	checkCrd = crd{ 0,0 }; //coordinate we want to move to, start at 0,0
	srand(time(0)); //seed 
	//Clear the lists just in case this is not the first run
	crdList.clear();
	roomList.clear();
	std::cout << "hitSelf = " << hitSelf << " turnBack = " << turnBack << " turnChance = " << turnChance << "\n";

	//mark 0,0 and 0,1 as taken (starting area is default)
	crd newCoord{ curX, curY }; //first coord is always 0,0
	crdList.emplace_back(newCoord);

	/* (Defaults)
	roomPathLength = 30; //Must be above 3 or more
	roomChance = 10; //% of paths will spawn a room
	turnChance = 50; //% chance to turn vs go in a straight line
	hitSelf = true; //true = path can go through itself
	turnBack = true; //true = path can turn backwards (requires hitSelf = true)
	*/


	//place roomPathLength amount of paths
	for (int i = 0; i < roomPathLength - 1; i++) { //start placing paths
		std::cout << "\nStarting loop \n";

		//Create a random list of rotations

		int randRotate[4] = { 0, 1, -1, 2 }; //0 = forwards , 1 = turn CW , -1 = turn CCW , 2 = backwards

		if (turnBack) { randRotate[3] = 0; } //If turning backwards is disabled, prevent that from being an option by making it check f twice.

		if (DiceRoll(turnChance)) { //If there is a chance to turn...
			std::random_shuffle(&randRotate[0], &randRotate[4]); //randomise the order they're checked in
			for (int i = 0; i < 4; i++) {
				if (randRotate[i] == 0) {
					std::swap(randRotate[i], randRotate[3]); //swap "forwards" to the end, so it is checked last.
				}
			}
		} //Could just randomly decide to go straight too.
		//Could change by looking for 0 and putting that at the back to ensure a straight line is the last option

		std::cout << "Random order = " << randRotate[0] << randRotate[1] << randRotate[2] << randRotate[3] << "\n";
		bool endMe = false; //for breaking the loop once a good location has been found

		//Create a list of directions to check, based on the list of rotations
		int checkDirs[4] = { Rotate(randRotate[0]), Rotate(randRotate[1]), Rotate(randRotate[2]), Rotate(randRotate[3]) }; //Could be simplified but this works fine.
		std::cout << "Random directions = " << checkDirs[0] << checkDirs[1] << checkDirs[2] << checkDirs[3] << "\n";

		//Check these directions one by one in the random order
		//As soon as one of these directions is found to be okay, break, move and start this loop again.
		for (int i = 0; i < 4; i++) { //check the 4 directions
			switch (checkDirs[i]) {
			case 0: // North
				if (endMe) break;
				directionOkay = true; //by default this direction is safe
				std::cout << "Checking North \n";
				if (!hitSelf) { //If it is not allowed to hit itself
					if (Exists(crd{ curX,curY + 1, 0 }, crdList)) //Search through array if that coordinate is taken)
					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
						std::cout << "North impossible \n";
						directionOkay = false; //this direction is not okay.
					}
				}
				if (directionOkay) {
					std::cout << "North is okay \n";
					curY++; //move up
					direction = checkDirs[i];
					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
					//Checking for duplicate coordinates has been removed, and instead, the program only checks for duplicates when placing actors for real
					//Removing constant checks to prevent creating duplicate coordinates speeds up generation time enourmously.
					crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
					endMe = true;
					break;
				}
			case 1: // East
				if (endMe) break;
				directionOkay = true; //by default this direction is safe
				std::cout << "Checking East \n";
				if (!hitSelf) { //If it is not allowed to hit itself
					if (Exists(crd{ curX + 1,curY, 0 }, crdList)) //Search through array if that coordinate is taken)
					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
						std::cout << "East impossible \n";
						directionOkay = false; //this direction is not okay.
					}
				}
				if (directionOkay) {
					std::cout << "East is okay \n";
					curX++; //move right
					direction = checkDirs[i];
					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
					crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
					endMe = true;
					break;
				}
			case 2: // South
				if (endMe) break;
				directionOkay = true; //by default this direction is safe
				std::cout << "Checking South \n";
				if (!hitSelf) { //If it is not allowed to hit itself
					if (Exists(crd{ curX,curY - 1, 0 }, crdList)) //Search through array if that coordinate is taken)
					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
						std::cout << "South impossible \n";
						directionOkay = false; //this direction is not okay.
					}
				}
				if (directionOkay) {
					std::cout << "South is okay \n";
					curY--; //move up
					direction = checkDirs[i];
					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
					crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
					endMe = true;
					break;
				}
			case 3: // West
				if (endMe) break;
				directionOkay = true; //by default this direction is safe
				std::cout << "Checking West \n";
				if (!hitSelf) { //If it is not allowed to hit itself
					if (Exists(crd{ curX - 1,curY, 0 }, crdList)) //Search through array if that coordinate is taken)
					{ //Double nested ifs here to ensure an array search doesnt happen if unneccessary.
						std::cout << "West impossible \n";
						directionOkay = false; //this direction is not okay.
					}
				}
				if (directionOkay) {
					std::cout << "West is okay \n";
					curX--; //move left
					direction = checkDirs[i];
					//if (!Exists(crd{ curX,curY, 0 }, crdList)) //Search through array if that coordinate is taken) (ensures no duplicate coords)
					crdList.emplace_back(crd{ curX, curY, 0 }); //add it to the list 
					endMe = true;
					break;
				}
			}
		}
		std::cout << "Floor has been placed. \n";

		MoveBorders(); //Check to see if the current position is out of the border, if so, increase border that way.

		if (DiceRoll(roomChance)) {
			std::cout << "!!!Placing a room!!! \n";

			//Room list is kept seperate from coord list,
			//so that the actual "snake" is seperate from the random rooms it drops
			//so that if hitself = false, it will still be able to move around 
			// (otherwise it would create a room, surround itself with floors, and be unable to move)
			//the final product will be both vectors combined to form a big list of floor coordinates
			//Can create different formations of rooms by adjusting these values, this is just a square.
			MakeRoom3x3(crd{ curX, curY, 0 });
			roomsPlaced++;
		}
	}
	flagLocation = crd{ curX, curY, 0 };

	//print out the list
	for (int i = 0; i < crdList.size(); i++) {
		std::cout << "crd #" << i
			<< "     x = " << crdList[i].x
			<< " y = " << crdList[i].y
			<< " Wall = " << crdList[i].d
			<< "\n";
	}

	crdList.insert(crdList.end(), roomList.begin(), roomList.end()); //concatonate the lists
	roomList.clear(); //clear that room list to save memory (its not needed now)


	//print out the list with rooms included
	std::cout << "WITH " << roomsPlaced << " ROOMS --------------------\n";
	for (int i = 0; i < crdList.size(); i++) {
		std::cout << "crd #" << i
			<< "     x = " << crdList[i].x
			<< " y = " << crdList[i].y
			<< " Wall = " << crdList[i].d
			<< "\n";
	}

	generateWalls();

	crdList.insert(crdList.end(), roomList.begin(), roomList.end()); //concatonate the lists
	roomList.clear(); //clear that room list to save memory (its not needed now)
	//roomlist was re used to hold the walls.

	//print out the list with walls included
	std::cout << "WITH " << wallsPlaced << " WALLS --------------------\n";
	for (int i = 0; i < crdList.size(); i++) {
		std::cout << "crd #" << i
			<< "     x = " << crdList[i].x
			<< " y = " << crdList[i].y
			<< " Wall = " << crdList[i].d
			<< "\n";
	}
}
void RoomGeneration::generateWalls()
{

	//Old method which prevented duplicate walls from being placed
	/*
	for (auto i : crdList) {

		//By default, generate a wall on blank space connected on NESW coordinates from the floor.
		std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";

		//Note:
		//roomList is used in this method to store walls.

		std::cout << "checking up\n";
		if (!Exists(crd{ i.x, i.y + 1, 0}, crdList) && !Exists(crd{ i.x, i.y + 1, 1 }, roomList)) { //UP
			roomList.emplace_back(crd{ i.x, i.y + 1, 1 });
			std::cout << "placed wall up\n"; wallsPlaced++;
		}

		std::cout << "checking right\n";
		if (!Exists(crd{ i.x + 1, i.y, 0 }, crdList) && !Exists(crd{ i.x + 1, i.y, 1 }, roomList)) { //RIGHT
			roomList.emplace_back(crd{ i.x + 1, i.y, 1 });
			std::cout << "placed wall right\n"; wallsPlaced++;
		}

		std::cout << "checking down\n";
		if (!Exists(crd{ i.x, i.y - 1 , 0}, crdList) && !Exists(crd{ i.x, i.y - 1 , 1 }, roomList)) { //DOWN
			roomList.emplace_back(crd{ i.x, i.y - 1, 1 });
			std::cout << "placed wall down\n"; wallsPlaced++;
		}

		std::cout << "checking left\n";
		if (!Exists(crd{ i.x - 1, i.y , 0}, crdList) && !Exists(crd{ i.x - 1, i.y , 1 }, roomList)) { //LEFT
			roomList.emplace_back(crd{ i.x - 1, i.y, 1 });
			std::cout << "placed wall left\n"; wallsPlaced++;
		}
	}
	if (placeCornerWalls) { //Seperated from the above loop, so it only checks this if statement once (instead of i:crdlist times)
		for (auto i : crdList) {

			//Corner walls means NE , SE , SW , NW
			std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";

			std::cout << "checking up right\n";
			if (!Exists(crd{ i.x+1, i.y + 1, 0 }, crdList) && !Exists(crd{ i.x+1, i.y + 1, 1 }, roomList)) { //UP RIGHT
				roomList.emplace_back(crd{ i.x+1, i.y + 1, 1 });
				std::cout << "placed wall up\n"; wallsPlaced++;
			}

			std::cout << "checking down right\n";
			if (!Exists(crd{ i.x + 1, i.y-1, 0 }, crdList) && !Exists(crd{ i.x + 1, i.y - 1, 1 }, roomList)) { //DOWN RIGHT
				roomList.emplace_back(crd{ i.x + 1, i.y - 1, 1 });
				std::cout << "placed wall right\n"; wallsPlaced++;
			}

			std::cout << "checking down left\n";
			if (!Exists(crd{ i.x - 1, i.y - 1 , 0 }, crdList) && !Exists(crd{ i.x - 1, i.y - 1 , 1 }, roomList)) { //DOWN LEFT
				roomList.emplace_back(crd{ i.x - 1, i.y - 1, 1 });
				std::cout << "placed wall down\n"; wallsPlaced++;
			}

			std::cout << "checking up left\n";
			if (!Exists(crd{ i.x - 1, i.y+1 , 0 }, crdList) && !Exists(crd{ i.x - 1, i.y + 1 , 1 }, roomList)) { //UP  LEFT
				roomList.emplace_back(crd{ i.x - 1, i.y + 1, 1 });
				std::cout << "placed wall left\n"; wallsPlaced++;
			}
		}
	}


	*/


	//New method does not care about duplicate walls, as explained in the other roomGen methods
	//   essentially, crds take up a tiny space in memory and it is not worth running tonnes of checks
	//   constantly to prevent duplicates from occuring
	//it is far faster to simply check for duplicates once, at the end of generation.
	//if memory space was a greater concern, the old methods would be preferred, but for this application, allowing duplicates is faster.

	//Of course, walls are not allowed to spawn on top of floors (the point of this method is to create a border of walls around all the floors)
	//, so one check is still required to occur.

	for (auto i : crdList) {
		//By default, generate a wall on any blank space connected on NESW coordinates from the floor.
		std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";

		//Note:
		//roomList is used in this method to store walls.

		std::cout << "checking up\n";
		if (!Exists(crd{ i.x, i.y + 1, 0 }, crdList)) { //UP
			roomList.emplace_back(crd{ i.x, i.y + 1, 1 });
			std::cout << "placed wall up\n"; wallsPlaced++;
		}

		std::cout << "checking right\n";
		if (!Exists(crd{ i.x + 1, i.y, 0 }, crdList)) { //RIGHT
			roomList.emplace_back(crd{ i.x + 1, i.y, 1 });
			std::cout << "placed wall right\n"; wallsPlaced++;
		}

		std::cout << "checking down\n";
		if (!Exists(crd{ i.x, i.y - 1 , 0 }, crdList)) { //DOWN
			roomList.emplace_back(crd{ i.x, i.y - 1, 1 });
			std::cout << "placed wall down\n"; wallsPlaced++;
		}

		std::cout << "checking left\n";
		if (!Exists(crd{ i.x - 1, i.y , 0 }, crdList)) { //LEFT
			roomList.emplace_back(crd{ i.x - 1, i.y, 1 });
			std::cout << "placed wall left\n"; wallsPlaced++;
		}
	}
	if (placeCornerWalls) { //Seperated from the above loop, so it only checks this if statement once (instead of i:crdlist times)
		for (auto i : crdList) {

			//Corner walls means NE , SE , SW , NW
			std::cout << "\nChecking for walls at x " << i.x << " y " << i.y << "\n";

			std::cout << "checking up right\n";
			if (!Exists(crd{ i.x + 1, i.y + 1, 0 }, crdList)) { //UP RIGHT
				roomList.emplace_back(crd{ i.x + 1, i.y + 1, 1 });
				std::cout << "placed wall up\n"; wallsPlaced++;
			}

			std::cout << "checking down right\n";
			if (!Exists(crd{ i.x + 1, i.y - 1, 0 }, crdList)) { //DOWN RIGHT
				roomList.emplace_back(crd{ i.x + 1, i.y - 1, 1 });
				std::cout << "placed wall right\n"; wallsPlaced++;
			}

			std::cout << "checking down left\n";
			if (!Exists(crd{ i.x - 1, i.y - 1 , 0 }, crdList)) { //DOWN LEFT
				roomList.emplace_back(crd{ i.x - 1, i.y - 1, 1 });
				std::cout << "placed wall down\n"; wallsPlaced++;
			}

			std::cout << "checking up left\n";
			if (!Exists(crd{ i.x - 1, i.y + 1 , 0 }, crdList)) { //UP  LEFT
				roomList.emplace_back(crd{ i.x - 1, i.y + 1, 1 });
				std::cout << "placed wall left\n"; wallsPlaced++;
			}
		}
	}
}
bool RoomGeneration::Exists(crd toFind, std::vector<crd> listToCheck)
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



int RoomGeneration::Rotate(int randomDirection)
{
	//Input rotation as 0, 2, +1, -1 aka F,B,CW,CCW 
	//newDirection & direction are absolute world orientations -> 0123 / NESW
	//Direction is the previously moved direction
	//New direction is the direction to look at next

   //Try to rotate CW or CCW by adding +1 or -1, might overflow/underflow at 3+1 or 0-1
   //Forwards changes nothing as F = +0, Backwards is +2 and might overflow.
	int newDirection = direction + randomDirection;

	//Resolve Overflows & Underflows
	if (newDirection == -1) newDirection = 3; //If North(0) rotates CCW (-1), loop back to West(3) (only case it will be -1)
	else if (newDirection == 4) newDirection = 0; //West(3) CW (4) goes North (0), and South(2) Backwards(4) goes North (0)
	else if (newDirection == 5) newDirection = 1; //West (3) backwards (5) overflows to east (1). (only case it will be 5)

	/* previous method, ignore as have made a shorter version.
	if (randomDirection == 1 || randomDirection == -1) { //If rotating CW or CCW
		if (newDirection == 4) newDirection == 0;
		else if (newDirection == -1) newDirection == 3;
	}
	else { //If moving Forwards or Backwards (forwards = +0 , backwards = +2)
		if (newDirection == 4) newDirection = 0; //If south(2) moves +2(4), loop back to north(0)
		else if (newDirection == 5) newDirection = 1; //If west(3) moves +2(5), loop back to east(1)
	}
	*/
	return newDirection; //Absolute world direction 0123 / NESW
}

void RoomGeneration::MakeRoom3x3(crd newCoord) //slow ish due to having to check for dupe coords
{
	/*
	0 0	0    x is the current node, 0 is the floors made around it
	0 x	0
	0 0	0
	*/

	//Old method which prevented coordinates having coordinates placed on top of them
	//Current method has moved checking for duplicates to roomToUnreal, where actors are placed for real
	//Decided that there's no real reason to care about duplicated coordinates, they take up such a tiny space in memory
	//	and having to constantly run checks to ensure there are no duplicates was slowing down generation
	//Generation was already fast, mind you, but moving checks to only be done once at the end for each object in the coordinate list,
	//	instead of having the checks occur every time a coordinate is checking to maybe be placed, speeds up time astronomically.
	/*
			if (!Exists(crd{ curX, curY + 1, 0 }, roomList)		&&		!Exists(crd{ curX, curY + 1, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX, curY + 1, 0 }); //TOP
			}
			if (!Exists(crd{ curX + 1, curY + 1, 0 }, roomList) &&		!Exists(crd{ curX + 1, curY + 1, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX + 1, curY + 1, 0 }); //TOP RIGHT
			}
			if (!Exists(crd{ curX + 1, curY, 0 }, roomList)		&&		!Exists(crd{ curX + 1, curY, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX + 1, curY, 0 }); //RIGHT
			}
			if (!Exists(crd{ curX + 1, curY - 1, 0 }, roomList) &&		!Exists(crd{ curX + 1, curY - 1, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX + 1, curY - 1, 0 }); //BOTTOM RIGHT
			}
			if (!Exists(crd{ curX, curY - 1, 0 }, roomList)		&&		!Exists(crd{ curX, curY - 1, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX, curY - 1, 0 }); //BOTTOM
			}
			if (!Exists(crd{ curX - 1, curY - 1, 0 }, roomList) &&		!Exists(crd{ curX - 1, curY - 1, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX - 1, curY - 1, 0 }); //BOTTOM LEFT
			}
			if (!Exists(crd{ curX - 1, curY, 0 }, roomList)		&&		!Exists(crd{ curX - 1, curY, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX - 1, curY, 0 }); //LEFT
			}
			if (!Exists(crd{ curX - 1, curY + 1, 0 }, roomList) &&		!Exists(crd{ curX - 1, curY + 1, 0 }, crdList)) //ensure no duplicate coords
			{
				roomList.emplace_back(crd{ curX - 1, curY + 1, 0 }); //TOP LEFT
			}
	*/
	roomList.emplace_back(crd{ curX, curY + 1, 0 }); //TOP
	roomList.emplace_back(crd{ curX + 1, curY + 1, 0 }); //TOP RIGHT
	roomList.emplace_back(crd{ curX + 1, curY, 0 }); //RIGHT
	roomList.emplace_back(crd{ curX + 1, curY - 1, 0 }); //BOTTOM RIGHT
	roomList.emplace_back(crd{ curX, curY - 1, 0 }); //BOTTOM
	roomList.emplace_back(crd{ curX - 1, curY - 1, 0 }); //BOTTOM LEFT	
	roomList.emplace_back(crd{ curX - 1, curY, 0 }); //LEFT
	roomList.emplace_back(crd{ curX - 1, curY + 1, 0 }); //TOP LEFT
}

int RoomGeneration::HardCodedRotate(int randomDirection)
{ //Input Fwd / Bck / CW / CCW  aka 0 2 1 3
	switch (direction)
	{
	case 0: //North
		if (randomDirection == 0) //forwards
			direction = 0;
		else if (randomDirection == 2) //backwards
			direction = 2;
		else if (randomDirection == 1) //turn CW
			direction = 1;
		else if (randomDirection == -1) //turn CCW
			direction = 3;
		break;
	case 1: //East
		if (randomDirection == 0) //forwards
			direction = 1;
		else if (randomDirection == 2) //backwards
			direction = 3;
		else if (randomDirection == 1) //turn CW
			direction = 2;
		else if (randomDirection == -1) //turn CCW
			direction = 0;
		break;
	case 2: //South
		if (randomDirection == 0) //forwards
			direction = 2;
		else if (randomDirection == 2) //backwards
			direction = 0;
		else if (randomDirection == 1) //turn CW
			direction = 3;
		else if (randomDirection == -1) //turn CCW
			direction = 1;
		break;
	case 3: //West
		if (randomDirection == 0) //forwards
			direction = 3;
		else if (randomDirection == 2) //backwards
			direction = 1;
		else if (randomDirection == 1) //turn CW
			direction = 0;
		else if (randomDirection == -1) //turn CCW
			direction = 2;
		break;
	}
	return direction; //Absolute direction to move in based on rotation
}
void RoomGeneration::MoveBorders()
{
	if (curX < minX) { //if this is the most left we have been
		minX = curX;
	}
	if (curX > maxX) { //if this is the most right we have been
		maxX = curX;
	}
	if (curY < minY) { //if this is the most down we have been
		minY = curY;
	}
	if (curY > maxY) { //if this is the most up we have been
		maxY = curY;
	}
}
bool RoomGeneration::DiceRoll(int percentage)
{
	if (percentage == 0) {
		return false;
	}
	return (FMath::RandRange(1, 100 / percentage) == 1 ? true : false);
}
