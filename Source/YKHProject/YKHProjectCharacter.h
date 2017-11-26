// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YKHProjectCharacter.generated.h"

UCLASS(Blueprintable)
class AYKHProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AYKHProjectCharacter();

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Pick();
	void Use();

	void Move(EAxis::Type Axis, float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};
