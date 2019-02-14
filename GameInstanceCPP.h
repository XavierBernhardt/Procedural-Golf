// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceCPP.generated.h"

/**
 * 
 */

struct mazeSettings {
	int width = 5;
	int height = 5;
	int length = 1;
	int deadends = false;
};

UCLASS()
class PROCEDURALGOLFV2_API UGameInstanceCPP : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "5", UIMin = "5"))
		int GImazeWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "5", UIMin = "5"))
		int GImazeHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "1", UIMin = "1"))
		int GIpathLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
		bool GInoDeadEndsAllowed = false;

	////--------SNAKE GEN---------\\\\

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Snake)
		int GIsnakeTrackLength; //0-100 chance to spawn a room of some kind

	////--------ROOM GEN---------\\\\

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
		int GIroomChance; //0-100 chance to spawn a room of some kind
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
		int GIturnChance; //0-100 chance to turn vs go in a straight line
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
		bool GIhitSelf = true; //true = path can go through itself 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
		bool GIturnBack = true; //true = path can turn backwards (requires hitSelf = true)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
		int GIroomPathLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int GIchanceForRock;//0 for no rocks, 100 for every space to be a rock.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
		bool GIplaceCornerWalls;//0 for no rocks, 100 for every space to be a rock.

	////--------CAVE GEN---------\\\\

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
		int GImaxCaveX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
		int GImaxCaveY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int GIcreateChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
		int GImaxCyclesInitial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
		int GImaxCyclesFinal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
		float GIminSizeMultiplier;

	mazeSettings getMazeSettings(); 


};

//
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "5", UIMin = "5"))
//int GImazeWidth = 5;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "5", UIMin = "5"))
//int GImazeHeight = 5;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze, meta = (ClampMin = "1", UIMin = "1"))
//int GIpathLength = 0;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
//bool GInoDeadEndsAllowed = false;
//
//////--------SNAKE GEN---------\\\\
//
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Snake)
//int GIsnakeTrackLength = 20; //0-100 chance to spawn a room of some kind
//
//////--------ROOM GEN---------\\\\
//
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
//int GIroomChance = 10; //0-100 chance to spawn a room of some kind
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
//int GIturnChance = 40; //0-100 chance to turn vs go in a straight line
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
//bool GIhitSelf = true; //true = path can go through itself 
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
//bool GIturnBack = true; //true = path can turn backwards (requires hitSelf = true)
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
//int GIroomPathLength = 30;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
//int GIchanceForRock = 25;//0 for no rocks, 100 for every space to be a rock.
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomGen)
//bool GIplaceCornerWalls = true;//0 for no rocks, 100 for every space to be a rock.
//
//////--------CAVE GEN---------\\\\
//
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
//int GImaxCaveX = 20;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "3", UIMin = "3"))
//int GImaxCaveY = 20;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
//int GIcreateChance = 41;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
//int GImaxCyclesInitial = 3;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
//int GImaxCyclesFinal = 3;
//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cave)
//float GIminSizeMultiplier = 0.35;

