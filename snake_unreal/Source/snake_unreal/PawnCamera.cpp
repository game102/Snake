// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "SnakeActor.h"
#include "AppleActor.h"

// Sets default values
APawnCamera::APawnCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MyCameraSpring = CreateDefaultSubobject<USpringArmComponent>("Spring");
	MyCameraSpring->SetRelativeLocation(FVector(0.f));
	MyCameraSpring->AttachTo(MyRootComponent);

	MyCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	MyCamera->AttachTo(MyCameraSpring, USpringArmComponent::SocketName);
	MyCamera->ProjectionMode =ECameraProjectionMode::Orthographic;
	MyCamera->OrthoWidth = 5000.f;

	MyCameraSpring->SetRelativeRotation( FRotator(-90.f, 0.f, 0.f) );
	MyCameraSpring->TargetArmLength = 1700.f;
	MyCameraSpring->bDoCollisionTest = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APawnCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APawnCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BuffterTimeApple += DeltaTime;

	if (GameMode > 0)
	{
		if (BuffterTimeApple > StepDelayApple)
		{
			if (!GamePause)
			{
				AddRandomApple();
			}
			BuffterTimeApple = 0.f;
		}
	}

}

// Called to bind functionality to input
void APawnCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("KeyMapMove", this, &APawnCamera::ForwardMove);

}

void APawnCamera::AddSnakeToMap()
{
	FVector StartPointLocation = GetActorLocation();
	FRotator StartPointRotation = GetActorRotation();

	if (GetWorld())
	{
		SnakeInstance = GetWorld()->SpawnActor<ASnakeActor>(StartPointLocation, StartPointRotation);
		GameMode = 1;  //Hide MainMenu Canvas
		SnakeInstance->WhoPawn = this;
	}
}

void APawnCamera::ForwardMove(float ButtonVal)
{
	Key = ButtonVal;

	if (GameMode > 0)
	{

		if (Key == 5)
		{
			GamePause = !GamePause;
		}

		if (!GamePause) {

			switch (Key)
			{
			case 1:
				if (WSDA.X != 1)
				{
					WSDA = FVector2D(0, 0);
					WSDA.X = -1;
				}
				break;

			case 2:
				if (WSDA.X != -1)
				{
					WSDA = FVector2D(0, 0);
					WSDA.X = 1;
				}
				break;

			case 3:
				if (WSDA.Y != 1)
				{
					WSDA = FVector2D(0, 0);
					WSDA.Y = -1;
				}
				break;

			case 4:
				if (WSDA.Y != -1)
				{
					WSDA = FVector2D(0, 0);
					WSDA.Y = 1;
				}
				break;
			}

			if (SnakeInstance)
			{
				SnakeInstance->DirectionMoveSnake = WSDA;
			}
		}
		else
		{
			SnakeInstance->DirectionMoveSnake = FVector2D(0, 0);
		}
	}
}


void APawnCamera::AddRandomApple()
{
	FRotator StartPointRotation = FRotator(0, 0, 0);

	float SpawnX = FMath::FRandRange(MinX, MaxX);
	float SpawnY = FMath::FRandRange(MinY, MaxY);

	FVector StartPoint = FVector(SpawnX, SpawnY, SpawnZ);

	if (SnakeInstance)
	{
		if (GetWorld())  //check actor is spawned
		{
			GetWorld()->SpawnActor<AAppleActor>(StartPoint, StartPointRotation);
		}
	}
}

int32 APawnCamera::GetScore()
{
	if (SnakeInstance)
	{
		return SnakeInstance->Score;
	}
	return 0;
}

void APawnCamera::SnakeDestory()
{
	GameMode = 0;

	if (SnakeInstance)
		SnakeInstance->Destroy(true, true);
}

