// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppleActor.generated.h"

UCLASS()
class SNAKE_UNREAL_API AAppleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAppleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class USphereComponent* MyRootComponent;

	class UStaticMesh* SnakeEatMesh;
	
	void CollectEat();

	UPROPERTY(EditAnywhere)
	TArray<AActor*> CollectedEats;

	float StepDelayEat = 2.0f;
	float BufferTimeEat = 0.f;
};
