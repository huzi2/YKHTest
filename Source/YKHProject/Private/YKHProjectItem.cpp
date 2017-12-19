#include "YKHProjectItem.h"
#include "Components/SphereComponent.h"

AYKHProjectItem::AYKHProjectItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("ItemMesh"));
	ItemMesh->CastShadow = true;
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	PickComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("PickComponent"));
	PickComponent->SetupAttachment(RootComponent);
	PickComponent->SetSphereRadius(50.f);

	this->SetActorScale3D(FVector(2.f, 2.f, 2.f));
}

void AYKHProjectItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
