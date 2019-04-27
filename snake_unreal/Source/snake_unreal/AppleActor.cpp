// Fill out your copyright notice in the Description page of Project Settings.

#include "AppleActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstance.h"
#include "SnakeActor.h"

// Sets default values
AAppleActor::AAppleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRootComponent = CreateDefaultSubobject<USphereComponent>("RootEatMesh");
	RootComponent = MyRootComponent;

	SnakeEatMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

	class UMaterialInstance* EatColor;
	EatColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/Material/PurColor_Apple.PurColor_Apple'")).Get();

	FVector Size = FVector(0.5f, 0.5f, 0.5f);

	class UStaticMeshComponent* EatChain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EatChain"));

	EatChain->SetStaticMesh(SnakeEatMesh);
	EatChain->SetRelativeScale3D(Size);
	EatChain->SetRelativeLocation(FVector(0, 0, 0));
	EatChain->SetMaterial(0, EatColor);
	EatChain->AttachTo(MyRootComponent);
	EatChain->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AAppleActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAppleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	 BufferTimeEat += DeltaTime;

	 if (BufferTimeEat > StepDelayEat)
	 {
		 Destroy(true, true);
	 }

	CollectEat();
}

void AAppleActor::CollectEat()
{

	GetOverlappingActors(CollectedEats);

	for (int32 i = 0; i < CollectedEats.Num(); ++i)
	{
		ASnakeActor* const Test = Cast<ASnakeActor>(CollectedEats[i]);

		if (Test)
		{
			Test->VisibleChainNum++;
			Test->Score++;

			Destroy(true, true);
			break;
		}
	}
}

