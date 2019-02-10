// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
using namespace::std;

/**
 * 
 */	struct node { //An element on the map
		int type = -1; //0 floor, 1 wall, 2 start floor, 3 end floor
		int group = -1; //What sub cavern it is within
	}; //Initialise both as -1 to avoid having to always set type & group when creating a node

	struct coord { //A location on the map
		int x, y; //Values on a 2D grid
	};

	struct cavern {
		int id; //The numerical ID of the cavern, eg 10
		int size; //The number of nodes in the cavern, eg  
		coord centre; //The coordinates of its centre
	};

	struct sflcoord { //Pair of coordinates to mark possible start/end positions
		coord start; //Player start coordinate
		coord flag; //Flag end coordinate
		float length; //Distance between start and flag
	};
class PROCEDURALGOLFV2_API CaveGeneration
{
public:
	int maxCaveX, maxCaveY; //Max size of the map horizontally & vertically
	int createChance; //Chance of a cell being a wall or cell at start
	int maxCyclesInitial; //How many cycles are going to be ran
	int currentGroup; //Used in labeling caverns, declared here so it can double as the value of the final group
	int groupFound; //Used in connecting caverns to show the cavern being connected to's value
	int cavernSize; //Used in labeling the size of the newly created cavern
	int biggestCaveID; //Used to store the value of the biggest cave (-1 is none are big enough)
	int biggestCaveSize; //Used to store the size of the biggest cave 
	int retryAttempts; //see below
	int maxCyclesFinal;
	bool needsRetry;
	int maxRetryAttempts; //How many attempts at creating a cave should be made (DEBUG ONLY - causes stack overflow if too high)
	int minSize; //The minimum size that the final tunnel system should be to be a valid map (usually like 30-40% of the total grid)
	//vector<centreNode> cavernCentres; //Used in storing the centres of all caverns
	vector<cavern> cavernList; //List of all caverns
	int cavernN, cavernE, cavernS, cavernW; //Used in storing the outer extremeties of each cavern for calculating the centre

	vector<vector<node>> caveGenerationBegin(); //Begins the generation
	vector<vector<node>> createMap(int maxX, int maxY); //Create the initial map
	vector<vector<node>> cycleMap(vector<vector<node>> oldMap, int maxX, int maxY); //Smooths the map out more every iteration
	void printMap(vector<vector<node>> map, int style); //Prints each cell either as a [style=0] floor/wall, or [style=1] by their group number 
	int nearbyWalls(vector<vector<node>> map, int x, int y); //Counts how many neighbouring walls are nearby

	vector<vector<node>> fillCavern(vector<vector<node>> map, int x, int y, int oldGroup, int newGroup); //Recursively discovers cavernns and adds every node found to a group
	vector<vector<node>> refillCaverns(vector<vector<node>> map, int x, int y, int newGroup); //Recursively discovers cavernns and adds every node found to a group
	vector<vector<node>> emptyCaverns(vector<vector<node>> map); //Resets the groups of every node to -1

	vector<vector<node>> destroyCavern(vector<vector<node>> map, int x, int y); //Recursively destroys a cavern from some coordinate (sets every node to be a wall)

	vector<vector<node>> connectCaverns(vector<vector<node>> map); //Joins all caverns together
	int searchForClosestCavern(vector<vector<node>> map, int x, int y, int group, int direction); //Recursively looks for the closest cavern, and connects them together.
	vector<vector<node>> tunnel(vector<vector<node>> map, int x, int y, int group, int direction); //Recursively tunnels towards the closest cavern


	vector<vector<node>> markCaverns(vector<vector<node>> map); //Mark each node in each seperated cave by their own individial number
	vector<coord> findPath(coord start, coord end); //Simple path finding algorithm, returns a list of coordinates to process (tunnel through)
	vector<vector<node>> generatePlayerAndFlag(vector<vector<node>> map); //Attempts to generate two coordinates for player start and the end flag

	vector<vector<node>> initCaveGen(int _maxCaveX, int _maxCaveY, int _createChance, int _maxCyclesInitial, int _maxCyclesFinal, float _minSizeMultiplier);

};