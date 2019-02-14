// Fill out your copyright notice in the Description page of Project Settings.

#include "CaveGeneration.h"
#include <iostream>
#include "Time.h"
#include <algorithm> //For randomising directions in findPath

using namespace::std;

vector<vector<node>> CaveGeneration::initCaveGen(int _maxCaveX, int _maxCaveY, int _createChance, int _maxCyclesInitial, int _maxCyclesFinal, float _minSizeMultiplier)
{

	maxCaveX = _maxCaveX; //Max size of the map horizontally
	maxCaveY = _maxCaveY; //Max size of the map vertically
	createChance = _createChance; //Chance of cell being a wall or cell at start
	maxCyclesInitial = _maxCyclesInitial; //How many cycles are going to be run at the start
	maxCyclesFinal = _maxCyclesFinal; //How many cycles are going to be run at the end
	minSize = (maxCaveX * maxCaveY) * _minSizeMultiplier; //Size of the final map should be at least this size (rounded down due to dividing an int)



	retryAttempts = 0;
	maxRetryAttempts = 0; //for debug

	vector<vector<node>> map;

	do {
		map = caveGenerationBegin();
	} while (needsRetry); //Ensures that the final map is at least a minimum size

	return map;
}



vector<vector<node>> CaveGeneration::markCaverns(vector<vector<node>> map) {

	currentGroup = 10; //Which cavern is currently being explored - 10 is used instead of 0 because its 2 digits, so prints nicely.

	for (auto x = 0; x < map.size(); x++) { //Loop through horizontal
		for (auto y = 0; y < map[0].size(); y++) { //Loop through vertical
			if (map[x][y].type == 0 && map[x][y].group == -1) {
				//map[x][y].group = currentGroup;
				cavernN = y; cavernE = x;  cavernS = y;  cavernW = x; //For finding the caves outer extremeties
				cavernSize = 0;
				map = fillCavern(map, x, y, -1, currentGroup); //Search for floors of group -1, and add them to a cavern group
				coord newCentre = coord{ (cavernN + cavernS) / 2 , (cavernW + cavernE) / 2 }; //The (rough int /2) centre coordinate of this cavern
				cavern newCavern = cavern{ currentGroup, cavernSize, newCentre }; //The cavern as an object labeled by its ID, containing a size and centre coordinate

				//cavernCentres.emplace_back(newCentre); 
				cavernList.emplace_back(newCavern);

				//Eg cavernCentres[0] is the x,y, coordinate of the centre of the cave group "10".
				currentGroup++; //All nodes connected to the old group have been marked, start using a new group for future seperate caverns
			}
		}
	}
	biggestCaveID = -1; //By default, none of the caves are big enough
	biggestCaveSize = -1; //By default, the first cave is the largest

	for (int i = 0; i < cavernList.size(); i++) { //Look to see if one of the caverns is big enough as is
		if (cavernList.at(i).size >= minSize && cavernList.at(i).size > biggestCaveSize) { //If this is the biggest cavern yet, and is at least x large
			biggestCaveID = i + 10; //Store the ID of the biggest cave
			biggestCaveSize = cavernList.at(i).size; //Store the size of the largest cave	
			std::cout << "======================Map okay////-/////-/-//-/////////-/-/-//////-/-/////--////////-///////--//////////////////-//////\n";
			std::cout << "Big enough cave found, size: " << cavernList.at(i).size << " ID: " << i + 10 << "\n";
			needsRetry = false;

			printMap(map, 0);
			printMap(map, 1);
		}
	}
	if (biggestCaveID != -1) { //If we have found at least one cavern that is big enough
		cavern tempCavern = cavernList.at(biggestCaveID - 10); //temporarily store the biggest cave
		cavernList.clear(); //clear all other caves from memory
		cavernList.emplace_back(tempCavern); //ensure the list only holds the biggest cave

		//Clear all other caves from the system
		for (auto x = 0; x < map.size(); x++) { //Loop through horizontal
			for (auto y = 0; y < map[0].size(); y++) { //Loop through vertical
				//if (map[x][y].type == 0 && map[x][y].group != biggestCaveID) { //Destroy any cave outside this size
				if (map[x][y].group != biggestCaveID) { //Destroy any cave outside this size
					map[x][y].type = 1;
					map[x][y].group = -1;
				}
			}
		}

	}
	else {
		needsRetry = true;
		std::cout << "======================MAP TOO SMALL, NEEEDS RETRY============================================================\n";
	}

	return map;
}

vector<coord> CaveGeneration::findPath(coord start, coord end)
{
	//This pathing algorithm does not reference the map at all, as it doesnt matter if the paths are floors or walls.
	vector<coord> pathFound; //A list of coordinates which connect two tunnels
	coord pos = start; //Begin at the first cave


	while (!(pos.x == end.x && pos.y == end.y)) { //Loop until a full path has been made

		int randDirs[4]; //Decide a random order to check directions in (helps prevent just straight lines)
		for (int i = 0; i < 4; i++) {
			randDirs[i] = i;
		}
		std::random_shuffle(&randDirs[0], &randDirs[4]);

		for (int i = 0; i < 4; i++) { //Randomly look to move in a direction (but still move in all directions eventually)
			switch (randDirs[i]) {
			case 0:
				if (pos.x > end.x) { //Are we to the right of the end coordinate
					pos.x--; //Move left
					pathFound.emplace_back(pos); //Add this new position to the list
				}
				break;
			case 1:
				if (pos.x < end.x) { //Are we to the left
					pos.x++; //Move right
					pathFound.emplace_back(pos);
				}
				break;
			case 2:
				if (pos.y > end.y) { //Are we above
					pos.y--; //Move down
					pathFound.emplace_back(pos);
				}
				break;
			case 3:
				if (pos.y < end.y) { //Are we below
					pos.y++; //Move up
					pathFound.emplace_back(pos);
				}
				break;
			}
		}

	}
	//pathFound.pop_back(); //Remove the final coordinate, as it'll just be == end
	std::cout << "Path found between start" << " x " << start.x << " y " << start.y << " and end " << " x " << end.x << " y " << end.y << "\n";
	for (auto i : pathFound) {
		std::cout << "x " << i.x << " y " << i.y << "\n";
	}
	return pathFound;
}

vector<vector<node>> CaveGeneration::generatePlayerAndFlag(vector<vector<node>> map) {
	//Attempt to generate two possible start/flag locations
	//Top left + bottom right || top right + bottom left
	//Walk towards the centre until a floor is hit for both
	//Check to see which of the two sets is larger
	//Set those coordinates for real
	//Could be replaced with A* algorithm, or similar to find a true path length


	sflcoord coord1 = { coord{0,0} , coord{maxCaveX,maxCaveY} , 0.f }; //Top left start / bottom right end
	sflcoord coord2 = { coord{maxCaveX, 0} , coord{0,maxCaveY} , 0.f }; //Top right start / bottom left end

	//Attempt to generate top left start / bottom right end ----------

	bool foundLand = false;
	while (!foundLand) {
		if ((coord1.start.x + maxCaveX * 0.05 >= maxCaveX) || (coord1.start.y + maxCaveY * 0.05 >= maxCaveY)) { //Will the move go out of bounds?
			coord1.start.x = cavernList.at(0).centre.x; //Set the player start to the cave centre (which must be a floor)
			coord1.start.y = cavernList.at(0).centre.y;
			foundLand = true;
		}
		else { //Will it move within bounds?
			coord1.start.x = (coord1.start.x + maxCaveX * 0.05); //Move 5% to the right
			coord1.start.y = (coord1.start.y + maxCaveY * 0.05); //Move 5% up
			if (map[coord1.start.x][coord1.start.y].type == 0) { //Have we hit land yet? (float to int conversion here to keep it on the grid)
				foundLand = true; //End the loop
			}
		}
	}

	foundLand = false;
	while (!foundLand) {
		if ((coord1.flag.x - maxCaveX * 0.05 <= 0) || (coord1.flag.y - maxCaveY * 0.05 <= 0)) { //Will the move go out of bounds?
			coord1.flag.x = cavernList.at(0).centre.x; //Set the flag to the cave centre (which must be a floor)
			coord1.flag.y = cavernList.at(0).centre.y;
			foundLand = true;
		}
		else { //Will it move within bounds?
			coord1.flag.x = (coord1.flag.x - maxCaveX * 0.05); //Move 5% to the left
			coord1.flag.y = (coord1.flag.y - maxCaveY * 0.05); //Move 5% down
			if (map[coord1.flag.x][coord1.flag.y].type == 0) { //Have we hit land yet? (float to int conversion here to keep it on the grid)
				foundLand = true; //End the loop
			}
		}
	}

	coord1.length = (coord1.flag.x - coord1.start.x) + (coord1.flag.y - coord1.start.y);

	//Attempt to generate top right start / bottom left end ----------

	foundLand = false;
	while (!foundLand) {
		if ((coord2.start.x - maxCaveX * 0.05 <= 0) || (coord2.start.y + maxCaveY * 0.05 >= maxCaveY)) { //Will the move go out of bounds?
			coord2.start.x = cavernList.at(0).centre.x; //Set the player start to the cave centre (which must be a floor)
			coord2.start.y = cavernList.at(0).centre.y;
			foundLand = true;
		}
		else { //Will it move within bounds?
			coord2.start.x = (coord2.start.x - maxCaveX * 0.05); //Move 5% to the left
			coord2.start.y = (coord2.start.y + maxCaveY * 0.05); //Move 5% down
			if (map[coord2.start.x][coord2.start.y].type == 0) { //Have we hit land yet? (float to int conversion here to keep it on the grid)
				foundLand = true; //End the loop
			}
		}
	}

	foundLand = false;
	while (!foundLand) {
		if ((coord2.flag.x + maxCaveX * 0.05 >= maxCaveX) || (coord2.flag.y - maxCaveY * 0.05 <= 0)) { //Will the move go out of bounds?
			coord2.flag.x = cavernList.at(0).centre.x; //Set the flag to the cave centre (which must be a floor)
			coord2.flag.y = cavernList.at(0).centre.y;
			foundLand = true;
		}
		else { //Will it move within bounds?
			coord2.flag.x = (coord2.flag.x + maxCaveX * 0.05); //Move 5% to the right
			coord2.flag.y = (coord2.flag.y - maxCaveY * 0.05); //Move 5% up
			if (map[coord2.flag.x][coord2.flag.y].type == 0) { //Have we hit land yet? (float to int conversion here to keep it on the grid)
				foundLand = true; //End the loop
			}
		}
	}

	coord2.length = (coord2.start.x - coord2.flag.x) + (coord2.flag.y - coord2.start.y);

	if (coord1.length > coord2.length) {
		map[coord1.start.x][coord1.start.y].type = 2; //Set those positions in the real map to be start (2) and end (3)
		map[coord1.flag.x][coord1.flag.y].type = 3;
	}
	else {
		map[coord2.start.x][coord2.start.y].type = 2;
		map[coord2.flag.x][coord2.flag.y].type = 3;
	}
	return map;
}

vector<vector<node>> CaveGeneration::connectCaverns(vector<vector<node>> map)
{
	//Have an array 
	//Cavern + its middle coordinate (or closest to middle if its like a C shape)
	//Loop through this array and connect each cavern together

		//Look from a coordinate in all 4 directions until a floor of a different group is discovered 
		//Once found, connect both caves together and perform a flood fill to mark them as joined

	if (cavernList.size() == 1) return map; //If there is only one cavern, no point connecting it to a non existent second one
	std::cout << "CONNECTING CAVERNS \n";
	/* //Old method
	for (int i = 0; i <= currentGroup-11; i++) { //Loop through all caverns (remember that caverns start at 10 for the purpose of printing)
		int direction = searchForClosestCavern(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y, i+10, -1); //Find a direction of the nearest cave, starting from the centre of the cavern.
		//i +10 is used here to convert the iterator back to the real group's value (remember that as caverns start at 10, if iterator is at 0, it is looking for cavern 10)

		//Location of nearest different cave: -1 / -2 nothing, 0 north, 1 east, 2 south, 3 west,
			if (direction != 1 && direction != 2 && groupFound !=0) { //Direction of closest cavern
				map = tunnel(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y, i+10, direction); //If a closest cavern was found, tunnel towards it
				map = fillCavern(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y, i+10, groupFound); //Mark both as connected
			}
			else {
				//If one cant be found, set every node in that cavern to be a wall (they are too disconnected)
				map = destroyCavern(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y); //Destroy this cavern from its centre outwards
			}
	}
	*/

	/* //Old method
	for (int i = 0; i <= currentGroup-11; i++) { //Loop through all caverns (remember that caverns start at 10 for the purpose of printing)
		int direction = searchForClosestCavern(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y, i+10, -1); //Find a direction of the nearest cave, starting from the centre of the cavern.
		//i +10 is used here to convert the iterator back to the real group's value (remember that as caverns start at 10, if iterator is at 0, it is looking for cavern 10)

		//Location of nearest different cave: -1 / -2 nothing, 0 north, 1 east, 2 south, 3 west,
			if (direction != 1 && direction != 2 && groupFound !=0) { //Direction of closest cavern
				map = tunnel(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y, i+10, direction); //If a closest cavern was found, tunnel towards it
				map = fillCavern(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y, i+10, groupFound); //Mark both as connected
			}
			else {
				//If one cant be found, set every node in that cavern to be a wall (they are too disconnected)
				map = destroyCavern(map, cavernList.at(i).centre.x, cavernList.at(i).centre.y); //Destroy this cavern from its centre outwards
			}
	}
	*/
	//vector<coord> allTunnelNodes;

	for (int connectFrom = 0; connectFrom < cavernList.size(); connectFrom++) { //Connect all caverns with a straight line
		for (int connectTo = 0; connectTo < cavernList.size(); connectTo++) { //Connect all caverns with a straight line
			if (connectFrom != connectTo) {
				std::cout << "CONNECTING \n" << connectFrom << " TO " << connectTo << "\n";


				vector<coord> newTunnel = findPath(cavernList.at(connectFrom).centre, cavernList.at(connectTo).centre);

				for (auto n : newTunnel) { //Turn every node into a floor
				//	printMap(map,0);
					map[n.y][n.x].type = 0;
					map[n.y][n.x].group = -1;

				}
				//Make the tunnels a bit thicker
				for (auto n : newTunnel) { //Turn every node into a floor

					int randOffset[4]; //Decide the offset of this tunnel thickener
					for (int i = 0; i < 4; i++) {
						randOffset[i] = i;
					}
					std::random_shuffle(&randOffset[0], &randOffset[4]);

					int newX, newY;

					switch (randOffset[0]) { //Generate an offset for this new node
					case 0:
						newX = +1;
						newY = +1;
						break;
					case 1:
						newX = -1;
						newY = -1;
						break;
					case 2:
						newX = +1;
						newY = -1;
						break;
					default:
						newX = -1;
						newY = +1;
					}

					if (n.y < maxCaveY - 1 && n.x < maxCaveX - 1) { //Place this new node on the map (overwrite whatever else was there before)
						map[n.y + newY][n.x + newX].type = 0;
						map[n.y + newY][n.x + newX].group = -1;
					}						//printMap(map, 0);

				}
			}
		}
		//	allTunnelNodes.(newTunnel);
	}
	cavernSize = 0;

	for (int i = 0; i < maxCyclesFinal; i++) {
		map = cycleMap(map, maxCaveX, maxCaveY);
	}

	map = emptyCaverns(map);
	map = markCaverns(map);


	//map = refillCaverns(map, cavernList.at(0).centre.x , cavernList.at(0).centre.y , currentGroup+1); //Set every cavern in the map to be a new unique type 

	//
	//map = cycleMap(map, maxX, maxY); //Smooth out the map a bit more
	//map = cycleMap(map, maxX, maxY); //Smooth out the map a bit more
	//map = cycleMap(map, maxX, maxY); //Smooth out the map a bit more
	//map = cycleMap(map, maxX, maxY); //Smooth out the map a bit more

	std::cout << "Map after caverns have been connected  - Walls/ Floor? \n";
	printMap(map, 0);
	std::cout << "Map after caverns have been connected - Group \n";
	printMap(map, 1);

	return map;
}

vector<vector<node>> CaveGeneration::tunnel(vector<vector<node>> map, int x, int y, int group, int direction)
{
	//Is the tunnel out of bounds
	if (x == 0 || x == maxCaveX - 1 || y == 0 || y == maxCaveY - 1) {
		return map; //Escape immidiately
	}
	//Is the tunnel now inside the new cavern?
	if (map[x][y].group == group) {
		return map; //Exit the loop
	}


	map[x][y].type = 0; //Set the current location to be a floor

	switch (direction) {
	case 0: //North
		map = tunnel(map, x, y + 1, group, direction); //Recursively tunnel north
		break;
	case 1: //East
		map = tunnel(map, x - 1, y, group, direction);
		break;
	case 2: //South
		map = tunnel(map, x, y - 1, group, direction);
		break;
	case 3: //West
		map = tunnel(map, x - 1, y, group, direction);
		break;
	}
	return map;
}

int CaveGeneration::searchForClosestCavern(vector<vector<node>> map, int x, int y, int group, int direction)
{
	//Looks in all 4 directions until a new cavern is discovered, then returns the direction it was found in.
	//map[x][y].group != group &&

	if (direction != -1 && direction != 2) { //If the closest direction has been found already
		return direction; //Return the direction that was found
	}

	if (x == 0 || x == maxCaveX - 1 || y == 0 || y == maxCaveY - 1 || direction == -2) { //Is the cell on the outer border?
		return -2;	//This direction is impossible
	}

	if ((map[x][y].type == 1) || (map[x][y].type == 0 && map[x][y].group == group)) { //We are in a wall, or the same cavern

		//check in 4 directions
		direction = searchForClosestCavern(map, x, y + 1, group, 0); //North
		if (direction == 4) return 0; //Return if this is the closest cavern

		direction = searchForClosestCavern(map, x + 1, y, group, 1); //East
		if (direction == 4) return 1;

		direction = searchForClosestCavern(map, x, y - 1, group, 2); //South
		if (direction == 4) return 2;

		direction = searchForClosestCavern(map, x - 1, y, group, 3); //West
		if (direction == 4) return 3;
	}

	else if (map[x][y].type == 0 && map[x][y].group != group) //We have found a new cavern
	{
		groupFound = map[x][y].group; //For flood filling later, save what group was found
		return 4; //4 means something has been found
		//End the recursive loop and pass up the chain that the direction taken to reach here is the shortest path
	}

	return direction; //Return the final closest direction
}

vector<vector<node>> CaveGeneration::fillCavern(vector<vector<node>> map, int x, int y, int oldGroup, int newGroup)
{
	if (x == 0 || x == maxCaveX - 1 || y == 0 || y == maxCaveY - 1) { //Is the cell on the outer border?
		return map;
	}

	//Extend the borders of the cavern (if neccessary)
	if (y > cavernN) cavernN = y;
	if (x > cavernE) cavernE = x;
	if (y < cavernS) cavernS = y;
	if (x < cavernW) cavernW = x;

	if (map[x][y].group == oldGroup && map[x][y].type == 0) //Check to see if the current node needs to be filled
	{
		map[x][y].group = newGroup;
		//	if (map[x+1][y].type = 0)
		map = fillCavern(map, x + 1, y, oldGroup, newGroup); //Recursively check the neighbouring cells.
//	if (map[x - 1][y].type = 0)
		map = fillCavern(map, x - 1, y, oldGroup, newGroup);
		//	if (map[x][y+1].type = 0)
		map = fillCavern(map, x, y + 1, oldGroup, newGroup);
		//	if (map[x][y-1].type = 0)
		map = fillCavern(map, x, y - 1, oldGroup, newGroup);
		cavernSize++;
	}

	else if (map[x][y].group == -1 && map[x][y].type == 0)
	{
		map[x][y].group = newGroup;
		map = fillCavern(map, x + 1, y, oldGroup, newGroup); //Recursively check the neighbouring cells.
		map = fillCavern(map, x - 1, y, oldGroup, newGroup);
		map = fillCavern(map, x, y + 1, oldGroup, newGroup);
		map = fillCavern(map, x, y - 1, oldGroup, newGroup);
		cavernSize++;
	}
	return map; //Return the map
}

vector<vector<node>> CaveGeneration::refillCaverns(vector<vector<node>> map, int x, int y, int newGroup)
{
	//Slightly different to fill cavern - this one simply converts every floor into one single group, with no other logic

	if (x == 0 || x == maxCaveX - 1 || y == 0 || y == maxCaveY - 1) { //Is the cell on the outer border?
		return map;
	}

	if (map[x][y].group != newGroup && map[x][y].type == 0) //Type == 0 uncommented, as some floors might be islanded (DEBUG) //Check to see if the current node needs to be filled
	{
		//if (map[x][y].type == 0) {
		map[x][y].group = newGroup;
		cavernSize++;
		//}

		map = refillCaverns(map, x + 1, y, newGroup); //Recursively check the neighbouring cells.
		map = refillCaverns(map, x - 1, y, newGroup);
		map = refillCaverns(map, x, y + 1, newGroup);
		map = refillCaverns(map, x, y - 1, newGroup);
	}

	return map; //Return the map
}

vector<vector<node>> CaveGeneration::emptyCaverns(vector<vector<node>> oldMap)
{
	vector<vector<node>> map = oldMap;

	cavernList.clear();

	for (auto x = 0; x < map.size(); x++) { //Loop through horizontal
		for (auto y = 0; y < map[0].size(); y++) { //Loop through vertical
			map[x][y].group = -1;
		}
	}

	return map;
}

vector<vector<node>> CaveGeneration::destroyCavern(vector<vector<node>> map, int x, int y)
{
	if (map[x][y].type == 0) //Check to see if the current node needs to be filled
	{
		map[x][y].type = 1; //Set it to be a wall
		map[x][y].group = -1; //Remove its group
		map = destroyCavern(map, x + 1, y); //Recursively destroy neighbouring cells
		map = destroyCavern(map, x - 1, y);
		map = destroyCavern(map, x, y + 1);
		map = destroyCavern(map, x, y - 1);
	}
	//Should never have an overflow/underflow as the outer edge of the map should always be a border of 1s
	return map; //Return the map
}

vector<vector<node>> CaveGeneration::caveGenerationBegin()
{
	needsRetry = false;
	srand(time(0)); //Re-seed the random generator



	std::cout << "Total number of nodes: " << (maxCaveX * maxCaveY) << ", Minimum cave size: " << minSize << "\n";
	cavernList.clear();
	groupFound = 0;

	vector<vector<node>> map = createMap(maxCaveX, maxCaveY); //Create an initial (noisy) map of maxX and maxY size

	for (auto c = 0; c <= maxCyclesInitial; c++) { //Cycle the smoothing method maxCycles amount of times
		std::cout << "Cycle " << c << "\n";
		printMap(map, 0);
		map = cycleMap(map, maxCaveX, maxCaveY);  //Run the map through the smooth method
	}
	std::cout << "Map after initial cycles \n";
	printMap(map, 0);

	map = markCaverns(map);	//Assign every node to a group based on which cavern it lies within

	std::cout << "Map after caverns have been decided \n";
	printMap(map, 0);
	printMap(map, 1);

	for (int i = 0; i < cavernList.size(); i++) {
		std::cout << "Centre of cavern: " << i << " x = " << cavernList.at(i).centre.x << ", y = " << cavernList.at(i).centre.y << "\n";
	}

	if (cavernList.size() != 1) {
		map = connectCaverns(map);	//If there are more than one caverns, connect them up
	}

	map = generatePlayerAndFlag(map); //Generate the start and flag coordinates


	std::cout << "FINAL MAP \n";
	printMap(map, 0);
	printMap(map, 1);


	//if (retryAttempts < maxRetryAttempts) {
	//	retryAttempts++;
	//	std::cout << "RETRY ATTEMPT " << retryAttempts << " ------------------------------------------------------------------------- \n";
	//	if (bigEnough == -1) { //Temporary fix to the "map is too small problem" - create a map repeatedly until one big enough is made
	//		map = caveGenerationBegin();

	//	}
	//}

	return map;
}

vector<vector<node>> CaveGeneration::createMap(int maxX, int maxY)
{
	vector<vector<node>> map; //The map being created
	map.resize(maxX, vector<node>(maxY)); //Resize the vector to the maximum x, y borders
	for (auto x = 0; x < maxX; x++) { //Loop through horizontal
		for (auto y = 0; y < maxY; y++) { //Loop through vertical
			if (x == 0 || x == maxX - 1 || y == 0 || y == maxY - 1) { //Is the cell on the outer border?
				map[x][y].type = 1; //Set it to always be a wall
			}
			else {
				if (rand() % 100 + 1 < createChance) { //Decide if it is a wall
					map[x][y].type = 1; //Yes-> Set cell to be a wall
				}
				else {
					map[x][y].type = 0; //No->  Set cell to be a floor
				}
			}
		}
	}
	return map; //Return this now pseudo randomised map
}

vector<vector<node>> CaveGeneration::cycleMap(vector<vector<node>> oldMap, int maxX, int maxY)
{
	vector<vector<node>> map;
	map.resize(maxX, vector<node>(maxY));
	int kill = 3; //How many floors nearby force this to become a floor
	int live = 4; //How many walls nearby force this to become a wall

	for (int x = 0; x < oldMap.size(); x++) { //Loop horizontally
		for (int y = 0; y < oldMap[0].size(); y++) { //Loop vertically
			int close = nearbyWalls(oldMap, x, y); //Find out how many walls are neighbouring this cell
			if (oldMap[x][y].type == 1) { //If this cell in the old map is a wall
				if (close < kill) map[x][y].type = 0; //If enough walls are nearby, become a floor
				else map[x][y].type = 1; //Otherwise, stay as a wall
			}
			else { //If this cell in the old map is a floor
				if (close > live) map[x][y].type = 1; //If enough walls are nearby, become a wall
				else map[x][y].type = 0; //Otherwise, stay as a floor
			}
		}
	}
	return map;
}

void CaveGeneration::printMap(vector<vector<node>> map, int style) { //Print the map
	std::cout << "Printing map of width " << map.size() << " by height " << map[0].size() << "\n";

	for (auto x = 0; x < map.size(); x++) { //Loop through horizontal
		for (auto y = 0; y < map[0].size(); y++) { //Loop through vertical
			if (style == 0) { //Based on each cell based on its type
				if (map[x][y].type == 0) { std::cout << "  "; } //Floor
				else if (map[x][y].type == 1) { std::cout << "# "; } //Wall
				else if (map[x][y].type == 2) { std::cout << "S "; } //Start
				else if (map[x][y].type == 3) { std::cout << "F "; } //Flag

			}
			else if (style == 1) { //Print out each cell based on its group
				std::cout << map[x][y].group << " ";
			}
		}
		std::cout << "\n"; //New line for new row
	}
}

int CaveGeneration::nearbyWalls(vector<vector<node>> map, int x, int y)
{
	int walls = 0;
	for (int i = -1; i <= 1; i++) { //Loop around the horizontal
		for (int j = -1; j <= 1; j++) { //Loop around the vertical
			int checkX = x + i; //Assign the checking values for horizontal
			int checkY = y + j; //Assign the checking values for vertical

			if (i == 0 && j == 0) { //This is the middle cell, do nothing
			}
			else if (checkX < 0 || checkY < 0 || checkX >= map.size() || checkY >= map[0].size()) { //If outside the border of the map, don't check (over/underflow)
				walls++;  //The outer border is considered to be a wall.
			}
			else { //If the cell being checked isnt outside the border, and isn't the middle cell
				walls += map[checkX][checkY].type; //Add +1 for 1 nearby, +0 for 0 nearby (1 = wall, 0 = floor)
			}
		}
	}
	return walls;
}