// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeActor.generated.h"

UCLASS()
class SNAKE_UNREAL_API ASnakeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 SnakeSize = 15;

	float StepSnake = 45.f;

	TArray<UStaticMeshComponent*> SnakeBody;

	class USphereComponent* MyRootComponent;

	void CreateSnakeBody();
	
	UPROPERTY(EditAnywhere)
	int32 VisibleChainNum = 3;

	void SetVisibleChain();

	UPROPERTY(EditAnywhere)
	FVector2D DirectionMoveSnake;

	float StepDelyMove = 0.5f;
	float BufferTimeMove = 0;

	void MoveSnake();

	int32 Score = 0;

	class APawnCamera* WhoPawn;

	void HaveDamage();
};
