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
#include "IceActor.h"
#include "SpringboardActor.h"
#include "FlagActor.h"

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
	slowMoving = false;
	dampingDefault = 1.2f;
	realDamping = dampingDefault;
	iceDamping = (dampingDefault / 3);
	overlappingIce = 0;
	JumpImpulse = 350000.0f;
	slowValue = 20; //velocities below this are considered slow
	touchedFlag = false;
	//cameraRotating = 0;
	cameraZooming = 0;


	//Makes a static mesh for the ball
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Meshes/BallStaticMesh.BallStaticMesh"));
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(realDamping);
	Ball->SetLinearDamping(realDamping);
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
	Camera->bUsePawnControlRotation = true; // We don't want the controller rotating the camera

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(100, 5.0f, FColor::White, TEXT("Begin"));

	FindComponentByClass<USphereComponent>()->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlap);
	FindComponentByClass<USphereComponent>()->OnComponentEndOverlap.AddDynamic(this, &APlayerPawn::OnOverlapEnd);
	FindComponentByClass<USphereComponent>()->OnComponentHit.AddDynamic(this, &APlayerPawn::OnHit);

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

	if (GetVelocity().Equals(stopVelocity) && canSetShoot && !touchedFlag) {
		canShoot = true;
		slowMoving = false;
	}

	if (slowMoving) {
		GEngine->AddOnScreenDebugMessage(200, 0.01f, FColor::Red, FString::Printf(TEXT("SlowMoving = true")));
		Ball->SetAngularDamping(15.f);
		Ball->SetLinearDamping(15.f);
	}
	else {
		GEngine->AddOnScreenDebugMessage(200, 0.01f, FColor::Green, FString::Printf(TEXT("SlowMoving = false")));
		Ball->SetAngularDamping(realDamping);
		Ball->SetLinearDamping(realDamping);
	}

	if (!canShoot && canSetShoot) { //nested if here to save doing the calcs below if unneccessary
		if (	(Ball->GetPhysicsLinearVelocity().X < slowValue) && (Ball->GetPhysicsLinearVelocity().X > -slowValue)	&&
				(Ball->GetPhysicsLinearVelocity().Y < slowValue) && (Ball->GetPhysicsLinearVelocity().Y > -slowValue)	&&
				//(Ball->GetPhysicsLinearVelocity().Z < slowValue) && (Ball->GetPhysicsLinearVelocity().Z > -slowValue)	)
				(Ball->GetPhysicsLinearVelocity().Z == 0))
		{
			slowMoving = true;
		}
	}
	GEngine->AddOnScreenDebugMessage(201, 0.01f, FColor::Blue, FString::Printf(TEXT("Angluar/Linear Damping=%f"), Ball->GetAngularDamping()));


	//FVector worldrotation = GetActorRotation().Vector();
	//AddMovementInput(worldrotation, 10.f, false);
	//Ball->AddImpulse();

	if (rotating != 0) {
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

	if (cameraRotating != 0) {
		//Camera->AddLocalRotation(FRotator(0.f, cameraRotating, 0.f));
		//Camera->AddRelativeLocation(FVector(0.f, cameraRotating, 0.f));



		// rotate around player
		FVector NewLocation = Ball->GetComponentLocation();

		AngleAxis += DeltaTime * Multiplier;

		if (AngleAxis >= 360.0f) AngleAxis = 0;	

		FVector RotateValue = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);
		NewLocation.X += RotateValue.X;
		NewLocation.Y += RotateValue.Y;
		NewLocation.Z += RotateValue.Z;
		FRotator NewRotation = FRotator(0, AngleAxis, 0);
		FQuat QuatRotation = FQuat(NewRotation);
		Camera->SetWorldLocationAndRotation(NewLocation, QuatRotation, false, 0, ETeleportType::None);

	}



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


	//PlayerInputComponent->BindAction("CameraRotateCW", IE_Pressed, this, &APlayerPawn::RotateCameraCW);
	//PlayerInputComponent->BindAction("CameraRotateCCW", IE_Pressed, this, &APlayerPawn::RotateCameraCCW);

	//PlayerInputComponent->BindAction("CameraRotateCW", IE_Released, this, &APlayerPawn::RotateCameraCWRelease);
	//PlayerInputComponent->BindAction("CameraRotateCCW", IE_Released, this, &APlayerPawn::RotateCameraCCWRelease);

	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &APlayerPawn::CameraZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &APlayerPawn::CameraZoomOut);

	PlayerInputComponent->BindAction("CameraZoomIn", IE_Released, this, &APlayerPawn::CameraZoomInRelease);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Released, this, &APlayerPawn::CameraZoomOutRelease);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawn::Shoot);

	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerPawn::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerPawn::LookUpAtRate);

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
		slowMoving = false;
		Ball->SetAngularDamping(realDamping);
		Ball->SetLinearDamping(realDamping);
		canShoot = false;
		canSetShoot = false;
		GetWorld()->GetTimerManager().SetTimer(canSetShootTimer, this, &APlayerPawn::canSetShootMethod, 1.0f, false, 1.0f);
	}
}

void APlayerPawn::canSetShootMethod()
{
	if (!touchedFlag)
	canSetShoot = true;
}
void APlayerPawn::flagTimerMethod()
{
	//logic for next level goes here
	canSetShoot = true;
	touchedFlag = false;
}

void APlayerPawn::OnOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<AIceActor>(OtherActor)) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, FString::Printf(TEXT("Overlap with ice begun")));
		overlappingIce++;
		realDamping = iceDamping;
		Ball->SetAngularDamping(realDamping);
		Ball->SetLinearDamping(realDamping);
	}
	if (Cast<ASpringboardActor>(OtherActor)) {

		if ((Ball->GetPhysicsLinearVelocity().X < slowValue) && (Ball->GetPhysicsLinearVelocity().X > -slowValue) &&
			(Ball->GetPhysicsLinearVelocity().Y < slowValue) && (Ball->GetPhysicsLinearVelocity().Y > -slowValue)) {}
		else{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Jumping")));
			const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
			Ball->AddImpulse(Impulse);
		}
	}
	if (Cast<AFlagActor>(OtherActor)) {
		if (!touchedFlag) {
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Touched Flag")));
			touchedFlag = true;
			slowMoving = true;
			canShoot = false;
			GetWorld()->GetTimerManager().SetTimer(flagTimer, this, &APlayerPawn::flagTimerMethod, 2.0f, false, 2.0f);
		}
	}

	//if (Cast<USpringboard>(OtherActor)) {
	//	GEngine->AddOnScreenDebugMessage(30, 0.01f, FColor::White, FString::Printf(TEXT("Colliding with springboard")));
	//}
}

void APlayerPawn::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AIceActor>(OtherActor)) {

		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::Printf(TEXT("Ending overlap with ice")));
		overlappingIce--;
		Ball->SetAngularDamping(realDamping);
		Ball->SetLinearDamping(realDamping);
		if (overlappingIce < 1) {
		//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("***Ending overlap with ALL ice")));
			realDamping = dampingDefault;
		}
	}
}

void APlayerPawn::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Cast<AIceActor>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hitting ice")));
	}
}

void APlayerPawn::RotateCameraCW()
{
	cameraRotating = 1;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("RotateCameraCW")));
}

void APlayerPawn::RotateCameraCCW()
{
	cameraRotating = -1;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("RotateCameraCCW")));
}

void APlayerPawn::RotateCameraCWRelease()
{
	if (cameraRotating > 0) {
		cameraRotating = 0;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("RotateCameraCWRelease")));
}

void APlayerPawn::RotateCameraCCWRelease()
{
	if (cameraRotating < 0) {
		cameraRotating = 0;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("RotateCameraCCWRelease")));
}

void APlayerPawn::CameraZoomIn()
{
	cameraZooming = 5; 
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("CameraZoomIn")));
}

void APlayerPawn::CameraZoomOut()
{
	cameraZooming = -5;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("CameraZoomOut")));
}

void APlayerPawn::CameraZoomInRelease()
{
	if (cameraZooming > 0) {
		cameraZooming = 0;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("CameraZoomInRelease")));
}

void APlayerPawn::CameraZoomOutRelease()
{
	if (cameraZooming < 0) {
		cameraZooming = 0;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("CameraZoomOutRelease")));
}