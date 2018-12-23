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
#include "Runtime/Core/Public/Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Tuned
	//defaultForce = 15;
	//maxForce = 30;
	//minForce = 3;
	//force = defaultForce;

	//Not Tuned
	canShoot = true;
	canSetShoot = true;
	rotating = 0;
	shootDirection = FRotator(0.f,0.f,0.f);


	//Makes a static mesh for the ball
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Meshes/BallStaticMesh.BallStaticMesh"));
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(1.5f);
	Ball->SetLinearDamping(1.5f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	//Makes a static mesh for the force visualiser (cylinder)
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CylMesh(TEXT("/Game/Meshes/ForceVisualiserMesh.ForceVisualiserMesh"));
	//Cyl = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cyl0"));
	//Cyl->SetStaticMesh(CylMesh.Object);
	//Cyl->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	//Cyl->SetRelativeScale3D(FVector(0.2f, 0.2f, 2.0f));
	//Cyl->BodyInstance.bLockYRotation;
	//Cyl->SetSimulatePhysics(false);
	//Cyl->SetVisibility(false);
	//Cyl->SetNotifyRigidBodyCollision(false);
	//Cyl->AttachTo(Ball);


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
	if (canShoot) {
		GEngine->AddOnScreenDebugMessage(77, 99.0f, FColor::White, FString::Printf(TEXT("CanShoot=True")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(77, 99.0f, FColor::White, FString::Printf(TEXT("CanShoot=False")));
	}
	//Neat trail accidentally made haha
	//FVector forwards = GetActorForwardVector() * 10;
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + forwards, FColor(255, 0, 0), false, 1.0f, 0, 12.333);

	//FVector forwards = Ball->GetPhysicsLinearVelocity();
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + forwards, FColor(255, 0, 0), false, 0.05f, 0, 12.333);

	float lineLonger = force * 20;
	FVector forwards = shootDirection.Vector()*lineLonger;

	if (canShoot)
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + forwards, FColor(0, 255, 0), false, 0.01f, 0, 12.333);
	else
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + forwards, FColor(255, 0, 0), false, 0.01f, 0, 12.333);


	GEngine->AddOnScreenDebugMessage(70, 99.0f, FColor::White, FString::Printf(TEXT("Velocity=%s"), *(GetVelocity().ToString())));
	GEngine->AddOnScreenDebugMessage(71, 99.0f, FColor::White, FString::Printf(TEXT("Rotation=%s"), *(GetActorRotation().ToString())));
	GEngine->AddOnScreenDebugMessage(72, 99.0f, FColor::White, FString::Printf(TEXT("Shoot Direction=%s"), *(shootDirection.ToString())));

	FVector stopVelocity = FVector(0.f, 0.f, 0.f);

	if (GetVelocity().Equals(stopVelocity) && canSetShoot) {
		canShoot = true;
	}
	//FVector worldrotation = GetActorRotation().Vector();
	//AddMovementInput(worldrotation, 10.f, false);
	//Ball->AddImpulse();

	if (rotating != 0){
		shootDirection.Yaw = shootDirection.Yaw + rotating * DeltaTime;
		if (shootDirection.Yaw > 360) {
			shootDirection.Yaw = 0;
		}
		if (shootDirection.Yaw < 0) {
			shootDirection.Yaw = 360;
		}
	}
	if (incrementForce != 0) {
		force = force + incrementForce * DeltaTime;
		if (force > maxForce) {
			force = maxForce;
		}
		if (force < minForce) {
			force = minForce;
		}
	}

	//if ((Ball->GetPhysicsLinearVelocity().X < 1) && (Ball->GetPhysicsLinearVelocity().Y < 1) && (Ball->GetPhysicsLinearVelocity().Z < 1) ) {
	//	GEngine->AddOnScreenDebugMessage(-1, 99.0f, FColor::White, FString::Printf(TEXT("AAAAA")));
	//	Ball->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	//}


}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("RotateCW", IE_Pressed, this, &APlayerPawn::RotateCW);
	PlayerInputComponent->BindAction("RotateCCW", IE_Pressed, this, &APlayerPawn::RotateCCW);

	PlayerInputComponent->BindAction("RotateCW", IE_Released, this, &APlayerPawn::RotateCWRelease);
	PlayerInputComponent->BindAction("RotateCCW", IE_Released, this, &APlayerPawn::RotateCCWRelease);

	PlayerInputComponent->BindAction("ForceAdd", IE_Pressed, this, &APlayerPawn::ForceAdd);
	PlayerInputComponent->BindAction("ForceRemove", IE_Pressed, this, &APlayerPawn::ForceRemove);

	PlayerInputComponent->BindAction("ForceAdd", IE_Released, this, &APlayerPawn::ForceAddRelease);
	PlayerInputComponent->BindAction("ForceRemove", IE_Released, this, &APlayerPawn::ForceRemoveRelease);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawn::Shoot);
}

void APlayerPawn::RotateCW()
{
	rotating = 200;
	//SetActorRelativeRotation(GetActorRotation() + rotator);
	//GetRootComponent()->SetRelativeRotation(GetRootComponent()->GetComponentRotation() + rotator);
	GEngine->AddOnScreenDebugMessage(101, 0.5f, FColor::White, TEXT("RotateCW"));
}

void APlayerPawn::RotateCCW()
{
	rotating = -200;
	//FRotator rotator = FRotator(0.f, 10.f, 0.f);
	//SetActorRelativeRotation(GetActorRotation() - rotator);
	GEngine->AddOnScreenDebugMessage(102, 0.5f, FColor::White, TEXT("RotateCCW"));
}

void APlayerPawn::RotateCWRelease()
{
	if (rotating > 0) {
		rotating = 0;
	}
}

void APlayerPawn::RotateCCWRelease()
{
	if (rotating < 0){
		rotating = 0;
	}
}

void APlayerPawn::ForceAdd()
{
	incrementForce = 20;
	GEngine->AddOnScreenDebugMessage(103, 0.5f, FColor::White, TEXT("ForceAdd"));
}

void APlayerPawn::ForceRemove()
{
	incrementForce = -20;
	GEngine->AddOnScreenDebugMessage(104, 0.5f, FColor::White, TEXT("ForceRemove"));
}

void APlayerPawn::ForceAddRelease()
{
	if (incrementForce > 0) {
		incrementForce = 0;
	}
}

void APlayerPawn::ForceRemoveRelease()
{
	if (incrementForce < 0) {
		incrementForce = 0;
	}
}

void APlayerPawn::Shoot()
{
	if (canShoot) {
		GEngine->AddOnScreenDebugMessage(105, 0.5f, FColor::White, TEXT("Shooting"));

		float trueForce = force * 100000;

		FVector forwards = shootDirection.Vector() * trueForce;
		FVector impulse = GetActorRotation().Vector() + forwards;
		//const FVector Impulse = FVector(0.f, force, 0.f);
		Ball->AddImpulse(impulse);

		canShoot = false;
		canSetShoot = false;
		GetWorld()->GetTimerManager().SetTimer(canSetShootTimer, this, &APlayerPawn::canSetShootMethod, 2.0f, false, 2.0f);
	}
}

void APlayerPawn::canSetShootMethod()
{
	canSetShoot = true;
}

