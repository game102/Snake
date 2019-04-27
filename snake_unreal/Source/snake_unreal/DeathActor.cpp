// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathActor.h"
#include "SnakeActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"

// Sets default values
ADeathActor::ADeathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	class UStaticMesh* WallMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object;

	WallMat = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/Material/PurColor_Wall.PurColor_Wall'")).Get();

	MyRootComponent = CreateDefaultSubobject<UBoxComponent>("RootEatMesh");
	RootComponent = MyRootComponent;

	class UStaticMeshComponent* WallChain;
	WallChain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WALL"));
	WallChain->SetStaticMesh(WallMesh);
	WallChain->SetRelativeLocation(FVector(0, 0, 0));
	WallChain->SetMaterial(0, WallMat);
	WallChain->AttachTo(MyRootComponent);
}

// Called when the game starts or when spawned
void ADeathActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADeathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CollideWall();
}

void ADeathActor::CollideWall()
{
	TArray<AActor*> CollectedActors;
	GetOverlappingActors(CollectedActors);

	for (int32 i = 0; i < CollectedActors.Num(); ++i)
	{
		ASnakeActor* const Test = Cast<ASnakeActor>(CollectedActors[i]);

		if (Test)
		{
			Test->HaveDamage();   //Destory snake
		}
		else
		{
			CollectedActors[i]->Destroy(true, true);
		}
	}
}

