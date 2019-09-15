// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidSpawner.h"
#include "Engine/World.h"
#include "BoidPawn.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"


// Sets default values
ABoidSpawner::ABoidSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	bounds = GetSpawnerBounds();
	UpdateBoundTransforms(bounds);
}

void ABoidSpawner::Tick(float DeltaSeconds)
{
	UpdateBoundVisibility();
}

// Called when the game starts or when spawned
void ABoidSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!BoidBlueprintType) return;

	for (int i = 0; i < TotalBoids; ++i)
	{
		SpawnBoid();
	}
}

void ABoidSpawner::SpawnBoid() const
{
	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector locationOffset = FVector(FMath::RandRange(-BoidAreaDepth / 2, BoidAreaDepth / 2),
		FMath::RandRange(-BoidAreaWidth / 2, BoidAreaWidth / 2),
		FMath::RandRange(-BoidAreaHeight / 2, BoidAreaHeight / 2));
	const FVector direction = FMath::VRand();
	GetWorld()->SpawnActor<ABoidPawn>(BoidBlueprintType.Get(), GetActorLocation() + locationOffset, direction.Rotation(), parameters);
}

FSpawnerBounds ABoidSpawner::GetSpawnerBounds() const
{
	TArray<UActorComponent*> components = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	TMap<FString, UStaticMeshComponent*> nameToMesh = TMap<FString, UStaticMeshComponent*>();
	for (UActorComponent* component : components)
	{
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(component);
		nameToMesh.Add(mesh->GetName(), mesh);
	}
	return FSpawnerBounds(nameToMesh.FindRef("LeftBound"), nameToMesh.FindRef("RightBound"), nameToMesh.FindRef("BackBound"),
		nameToMesh.FindRef("FrontBound"), nameToMesh.FindRef("TopBound"), nameToMesh.FindRef("BottomBound"));
}

void ABoidSpawner::UpdateBoundTransforms(FSpawnerBounds bounds) const
{
	const FVector yLocation = FVector(0, -BoidAreaWidth / 2, 0);
	const FVector yScale = FVector(BoidAreaDepth / 100, 1, BoidAreaHeight / 100);
	SetBoundTransform(bounds.LeftBound, yLocation, yScale);
	SetBoundTransform(bounds.RightBound, -yLocation, yScale);

	const FVector xLocation = FVector(BoidAreaDepth / 2, 0, 0);
	const FVector xScale = FVector(1, BoidAreaWidth / 100, BoidAreaHeight / 100);
	SetBoundTransform(bounds.FrontBound, xLocation, xScale);
	SetBoundTransform(bounds.BackBound, -xLocation, xScale);

	const FVector zLocation = FVector(0, 0, BoidAreaHeight / 2);
	const FVector zScale = FVector(BoidAreaDepth / 100, BoidAreaWidth / 100, 1);
	SetBoundTransform(bounds.TopBound, zLocation, zScale);
	SetBoundTransform(bounds.BottomBound, -zLocation, zScale);
}

void ABoidSpawner::SetBoundTransform(UStaticMeshComponent* bound, FVector location, FVector scale)
{
	if (!bound) return;
	bound->SetRelativeLocation(location);
	bound->SetRelativeScale3D(scale);
}

void ABoidSpawner::UpdateBoundVisibility()
{
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (!controller) return;
	AActor* camera = controller->GetViewTarget();
	if (!camera) return;
	for (UStaticMeshComponent* bound : bounds.ToArray())
	{
		bound->SetHiddenInGame(!IsBoundVisible(bound, camera));
	}
}

bool ABoidSpawner::IsBoundVisible(UStaticMeshComponent* bound, AActor* camera)
{
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(camera);
	params.AddIgnoredComponent(bound);
	return GetWorld()->LineTraceSingleByChannel(hitResult, bound->GetComponentLocation(), camera->GetActorLocation(),
		ECC_Camera, params);
}

