// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeNodeMain.generated.h"

UCLASS()
class PROCEDURALGOLFV2_API AMazeNodeMain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeNodeMain();
	//	AMazeNodeMain(int type, int floor);


protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	virtual void Destroyed() override;
	int type;
	int floor;
	bool inverted;
	bool heightChanged;
	float newHeight;
	AActor* myPiece;
	AActor* myFloor;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeN;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeC;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeL;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeI;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeT;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeX;

	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeFloor;
	UPROPERTY(EditAnywhere, Category = Maze1)
		TSubclassOf<class AActor> MazeFloorHole;

	//Maze 2 (Curved)

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeNAlt2;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeCAlt2;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeLAlt2;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeIAlt2;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeTAlt2;

	UPROPERTY(EditAnywhere, Category = MazeAlt1)
		TSubclassOf<class AActor> MazeXAlt2;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt2N;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt2C;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt2L;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt2I;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt2T;

	UPROPERTY(EditAnywhere, Category = MazeAlt1Chance, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForAlt2X;

	//Room

	UPROPERTY(EditAnywhere, Category = Room)
		TSubclassOf<class AActor> MazeWallSAND;

	UPROPERTY(EditAnywhere, Category = Room)
		TSubclassOf<class AActor> MazeFloorSAND;

	UPROPERTY(EditAnywhere, Category = Room)
		TSubclassOf<class AActor> MazeFloorSAND2;

	//Cave

	UPROPERTY(EditAnywhere, Category = Cave)
		TSubclassOf<class AActor> MazeWallCave;

	UPROPERTY(EditAnywhere, Category = Cave)
		TSubclassOf<class AActor> MazeFloorCave;

	//Snake ramps


	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> IRampWall;

	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> LRampWall;

	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> IRampFloor;

	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> LRampFloor;


	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> LRampFloorInverted;

	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> LRampWallInverted;

	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> IRampInverted;

	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> IRampNorth;

	UPROPERTY(EditAnywhere, Category = Snake)
		TSubclassOf<class AActor> IRampNorthInverted;




	UPROPERTY(EditAnywhere, Category = Room, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int ChanceForSandFloorAlt;

	void setType(int setPiece);
	void setFloor(int setFloor);
	void init();
	bool DiceRoll(int percentage);
	void invert();
	void changeHeight(float heightMultiplier);
};
