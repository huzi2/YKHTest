// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YKHProjectItem.h"
#include "YKHProjectCharacter.generated.h"

UCLASS(Blueprintable)
class AYKHProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AYKHProjectCharacter();

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "AYKHProjectCharacter")
	WEAPON_TYPE GetEquipWeaponType() const;

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

private:
	void Pick();
	void Use();

	void Move(EAxis::Type Axis, float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PlayAnim(class UAnimMontage* Anim);

	UFUNCTION()
	void StopAnim(class UAnimMontage* Anim);

	class AYKHProjectItem* GetNearItems() const;
	void AttachWeapon();
	void DetachWeapon();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* PickComponent;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	class UAnimMontage* PickAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	class UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	class AYKHProjectItem* WeaponItem;

private:
	bool DuringPick;
	bool DuringUse;
};
