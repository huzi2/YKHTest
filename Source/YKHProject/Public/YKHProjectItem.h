// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YKHProjectItem.generated.h"

UCLASS()
class YKHPROJECT_API AYKHProjectItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AYKHProjectItem(const class FObjectInitializer& ObjectInitializer);

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
	class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
	class USphereComponent* PickComponent;
};
