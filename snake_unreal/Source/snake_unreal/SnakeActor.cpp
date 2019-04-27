// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakeActor.h"
#include "PawnCamera.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
ASnakeActor::ASnakeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRootComponent = CreateDefaultSubobject<USphereComponent>("MyRoot");
	RootComponent = MyRootComponent;

	FVector PosActor = GetActorLocation();
	MyRootComponent->SetRelativeLocation(PosActor);

	CreateSnakeBody();
}

// Called when the game starts or when spawned
void ASnakeActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetVisibleChain();

	BufferTimeMove += DeltaTime;
	
	if (BufferTimeMove > StepDelyMove)
	{
		MoveSnake();
		BufferTimeMove = 0;
	}
}

void ASnakeActor::CreateSnakeBody()
{
	class UStaticMesh* SnakeChainMesh;
	SnakeChainMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>( TEXT("/Engine/BasicShapes/Sphere") ).Object;

	class UMaterialInstance* SnakeBodyColor;
	SnakeBodyColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>( TEXT("MaterialInstanceConstant'/Game/Material/PurColor_Snake.PurColor_Snake'") ).Get();

	class UMaterialInstance* SnakeHeadColor;
	SnakeHeadColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/Material/PurColor_Apple.PurColor_Apple'")).Get();

	FVector NextPoint = GetActorLocation();
	FName NameChain;
	FString TheStirng;

	for (int32 i = 0; i < SnakeSize; i++)
	{
		TheStirng = "Chain" + FString::FromInt(i);
		NameChain = FName(*TheStirng);

		class UStaticMeshComponent* BodyChain = CreateDefaultSubobject<UStaticMeshComponent>(NameChain);

		BodyChain->SetStaticMesh(SnakeChainMesh);
		BodyChain->SetRelativeLocation(NextPoint);

		SnakeBody.Add(BodyChain);

		NextPoint.X -= StepSnake;

		BodyChain->AttachTo(MyRootComponent);
		BodyChain->SetMaterial(0, SnakeBodyColor);

		if (i == 0)
		{
			BodyChain->SetMaterial(0, SnakeHeadColor); 
		}
		else
		{
			BodyChain->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	SetVisibleChain();
}

void ASnakeActor::SetVisibleChain()
{
	for (int32 IndexShow = 0; IndexShow < SnakeBody.Num(); IndexShow++)
	{
		if (IndexShow < VisibleChainNum)
		{
			SnakeBody[IndexShow]->SetVisibility(true, true);
		}
		else
		{
			SnakeBody[IndexShow]->SetVisibility(false, true);
		}
	}
}

void ASnakeActor::MoveSnake()
{
	//X -1 UP  Y -1 Right
	if ((DirectionMoveSnake.X != 0) || (DirectionMoveSnake.Y != 0))
	{
		for (int Chain = SnakeBody.Num() - 1; Chain > 0; Chain--)
		{
			FVector V = SnakeBody[Chain - 1]->RelativeLocation;
			SnakeBody[Chain]->SetRelativeLocation(V);
		}

		FVector StartPoint = SnakeBody[0]->RelativeLocation;

		if (DirectionMoveSnake.X > 0)
			StartPoint.X -= StepSnake;

		if (DirectionMoveSnake.X < 0)
			StartPoint.X += StepSnake;

		if (DirectionMoveSnake.Y > 0)
			StartPoint.Y -= StepSnake;

		if (DirectionMoveSnake.Y < 0)
			StartPoint.Y += StepSnake;

		SnakeBody[0]->SetRelativeLocation(StartPoint);
	}
}

void ASnakeActor::HaveDamage()
{
	WhoPawn->SnakeDestory();
}

