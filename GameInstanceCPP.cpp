// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstanceCPP.h"
#include "Engine.h"


mazeSettings UGameInstanceCPP::getMazeSettings()
{
	if (GImazeWidth < 5) GImazeWidth = 5;
	if (GImazeHeight < 5) GImazeHeight = 5;
	//int checkWidth = GImazeWidth;
	//int checkHeight = GImazeHeight;

	//Ensure only odd numbers go through (even ones generate maps with an unused row/column
	//if (checkWidth % 2 == 0)
	//	GImazeWidth++;
	//if (checkHeight % 2 == 0)
	//	GImazeHeight++;
	//No checking for maximum here, as in the settings menu it is limited to 99 anyway
	
	return mazeSettings{ GImazeWidth, GImazeHeight, GIpathLength, GInoDeadEndsAllowed };
}
snakeSettings UGameInstanceCPP::getSnakeSettings()
{
	return snakeSettings{ GIsnakeTrackLength, GIsnakeHeightMultiplier , GIsnakeAllowDown ,GIsnakeAllowUp , GIsnakeHeightChance };
}
roomSettings UGameInstanceCPP::getRoomSettings()
{
	return roomSettings{ GIroomChance, GIturnChance, GIhitSelf, GIturnBack, GIroomPathLength, GIchanceForRock, GIplaceCornerWalls };
}

caveSettings UGameInstanceCPP::getCaveSettings()
{
	return caveSettings{GImaxCaveX, GImaxCaveY, GIcreateChance, GImaxCyclesInitial, GImaxCyclesFinal, GIminSizeMultiplier};
}

controlSettings UGameInstanceCPP::getControlSettings()
{
	return controlSettings{ GIForceMultiplier , GIDampingMultiplier , GIJumpMultiplier, GISensitivityMultiplier, GIMassMultiplier };
}
