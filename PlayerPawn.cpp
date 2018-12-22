// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Engine.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMesh.h"
#include "EngineGlobals.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Tuned
	//defaultForce = 100000;
	//maxForce = 150000;
	//minForce = 50000;
	//force = defaultForce;

	//Not Tuned
	canShoot = true;

	//Makes a static mesh for the ball
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Meshes/BallStaticMesh.BallStaticMesh"));
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(1.0f);
	Ball->SetLinearDamping(1.0f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;


	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(100, 5.0f, FColor::White, TEXT("Begin"));

}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(77, 99.0f, FColor::White, FString::Printf(TEXT("canShoot =  %s"), canShoot));
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("RotateCW", IE_Pressed, this, &APlayerPawn::RotateCW);
	PlayerInputComponent->BindAction("RotateCCW", IE_Pressed, this, &APlayerPawn::RotateCCW);
	PlayerInputComponent->BindAction("ForceAdd", IE_Pressed, this, &APlayerPawn::ForceAdd);
	PlayerInputComponent->BindAction("ForceRemove", IE_Pressed, this, &APlayerPawn::ForceRemove);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawn::Shoot);
}

void APlayerPawn::RotateCW()
{
	GEngine->AddOnScreenDebugMessage(101, 0.5f, FColor::White, TEXT("RotateCW"));
}

void APlayerPawn::RotateCCW()
{
	GEngine->AddOnScreenDebugMessage(102, 0.5f, FColor::White, TEXT("RotateCCW"));
}

void APlayerPawn::ForceAdd()
{
	GEngine->AddOnScreenDebugMessage(103, 0.5f, FColor::White, TEXT("ForceAdd"));
}

void APlayerPawn::ForceRemove()
{
	GEngine->AddOnScreenDebugMessage(104, 0.5f, FColor::White, TEXT("ForceRemove"));
}

void APlayerPawn::Shoot()
{
	GEngine->AddOnScreenDebugMessage(105, 0.5f, FColor::White, TEXT("Shoot"));

	if (canShoot) {
		const FVector Impulse = FVector(force, 0.f, 0.f);
		Ball->AddImpulse(Impulse);
		canShoot = false;
	}
}

