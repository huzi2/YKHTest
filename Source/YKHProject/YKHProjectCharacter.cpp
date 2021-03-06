﻿// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "YKHProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Engine.h"
//#include "AnimNode_BlendListByInt.h"
//#include "AnimNodes/AnimNode_BlendListByInt.h"
//#include "AnimGraphNode_BlendListByInt.h"
//#include "AnimGraphNode_BlendListByInt.h"
//#include "Runtime/AnimGraphRuntime/Public/AnimNodes/AnimNode_BlendListByInt.h"
//#include "Runtime/AnimGraphRuntime/Public/AnimNodes/AnimNode_BlendListBase.generated.h"
//#include "Engine/Source/Runtime/AnimGraphRuntime/Public/AnimNodes/AnimNode_BlendListByInt.h"
//#include "AnimNode_BlendListByInt.h"

AYKHProjectCharacter::AYKHProjectCharacter()
	: DuringPick(false)
	, DuringUse(false)
	, PlayAnimIndex(0)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 42.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PickComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PickComponent"));
	PickComponent->SetupAttachment(RootComponent);
	PickComponent->SetSphereRadius(70.f);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AYKHProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

WEAPON_TYPE AYKHProjectCharacter::GetEquipWeaponType() const
{
	if (WeaponItem != nullptr)
	{
		return WeaponItem->GetWeaponType();
	}
	return WEAPON_TYPE::WEAPON_OFF;
}

void AYKHProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Pick"), IE_Pressed, this, &AYKHProjectCharacter::Pick);
	PlayerInputComponent->BindAction(TEXT("Use"), IE_Pressed, this, &AYKHProjectCharacter::Use);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AYKHProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AYKHProjectCharacter::MoveRight);
}

void AYKHProjectCharacter::Jump()
{
	if (DuringPick == true)
	{
		return;
	}

	Super::Jump();
}

void AYKHProjectCharacter::Pick()
{
	if (DuringPick == true)
	{
		return;
	}
	if (DuringUse == true)
	{
		return;
	}
	if (GetMovementComponent()->IsFalling() == true)
	{
		return;
	}

	if (WeaponItem == nullptr)
	{
		// 모든 액터에서 확인하는 코드..나중에 필요할지도 모르니 냅둠
		//TArray<AActor*> FoundActors;
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AYKHProjectItem::StaticClass(), FoundActors);
		//WeaponItem = Cast<AYKHProjectItem>(FoundActors[0]);

		WeaponItem = GetNearItems();
	}

	if (WeaponItem == nullptr)
	{
		return;
	}

	AttachWeapon();

	PlayAnim(PickAnim);
	DuringPick = true;
}

void AYKHProjectCharacter::Use()
{
	if (DuringPick == true)
	{
		return;
	}
	if (DuringUse == true)
	{
		return;
	}

	PlayAnim(TEXT(""));

	//DetachWeapon();

	//PlayAnim(AttackAnim);
	//PlayAnim(TEXT("Attack"));
	PlayAnimIndex = 1;
	//DuringUse = true;
}

void AYKHProjectCharacter::Move(EAxis::Type Axis, float Value)
{
	if (DuringPick == true)
	{
		return;
	}
	if (DuringUse == true)
	{
		return;
	}
	if (Controller == nullptr)
	{
		return;
	}
	if (Value == 0.f)
	{
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FVector Direction = FRotationMatrix(FRotator(0.f, Rotation.Yaw, 0.f)).GetUnitAxis(Axis);
	AddMovementInput(Direction, Value);
}

void AYKHProjectCharacter::MoveForward(float Value)
{
	Move(EAxis::X, Value);
}

void AYKHProjectCharacter::MoveRight(float Value)
{
	Move(EAxis::Y, Value);
}

void AYKHProjectCharacter::PlayAnim(UAnimMontage* Anim)
{
	const float AnimDuration = PlayAnimMontage(Anim);

	FTimerHandle AnimTimerHandle;
	FTimerDelegate StopTimerDelegate;
	StopTimerDelegate.BindUFunction(this, TEXT("StopAnim"), Anim);
	GetWorldTimerManager().SetTimer(AnimTimerHandle, StopTimerDelegate, AnimDuration, false);
}

void AYKHProjectCharacter::PlayAnim(const FString& AnimName)
{
	/*if (AnimList.Contains(AnimName) == false)
	{
		return;
	}

	UAnimSequence* AnimSequence = *AnimList.Find(AnimName);

	if (AnimSequence == nullptr)
	{
		return;
	}*/

	/*GetMesh()->PlayAnimation(AnimSequence, false);
	const float AnimDuration = AnimSequence->GetMaxCurrentTime();

	FTimerHandle AnimTimerHandle;
	FTimerDelegate StopTimerDelegate;
	StopTimerDelegate.BindUFunction(this, TEXT("PlayAnim"), AnimName);
	GetWorldTimerManager().SetTimer(AnimTimerHandle, StopTimerDelegate, AnimDuration, false);*/

	if (TestAni == nullptr)
	{
		return;
	}

	//GetMesh()->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(TestAni, TEXT("Default"));

	GetMesh()->PlayAnimation(TestAni, false);
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	const float AnimDuration = TestAni->GetMaxCurrentTime();

	FTimerHandle AnimTimerHandle;
	GetWorldTimerManager().SetTimer(AnimTimerHandle, this, &AYKHProjectCharacter::StopAnim, AnimDuration, false);

	IAnimClassInterface* AnimClassInterface = IAnimClassInterface::GetFromClass(GetClass());

	if (AnimClassInterface == nullptr)
	{
		return;
	}

	const TArray<UStructProperty*>& AnimNodeProperties = AnimClassInterface->GetAnimNodeProperties();
	for (UStructProperty* Property : AnimNodeProperties)
	{
		// 따로 애니메이션용 UObject를 만들어줘야할듯?
		//if (Property->Struct->IsChildOf(FAnimNode_BlendListByInt::StaticStruct()) == true)
		{
		//	//Property->Struct->GetClass<UAnimSequence*>();
		}
	}
}

void AYKHProjectCharacter::StopAnim(UAnimMontage* Anim)
{
	DuringPick = false;
	DuringUse = false;

	StopAnimMontage(Anim);
}

void AYKHProjectCharacter::StopAnim()
{
	DuringPick = false;
	DuringUse = false;

	GetMesh()->Stop();
	GetMesh()->PlayAnimation(IdleAni, true);
}

AYKHProjectItem* AYKHProjectCharacter::GetNearItems() const
{
	if (PickComponent == nullptr)
	{
		return nullptr;
	}

	TArray<AActor*> NearItems;
	PickComponent->GetOverlappingActors(NearItems);

	for (auto PickItem : NearItems)
	{
		AYKHProjectItem* PickItemTemp = Cast<AYKHProjectItem>(PickItem);

		// 여러 개일 경우 순서 처리 아직 미정
		if (PickItemTemp != nullptr)
		{
			return PickItemTemp;
		}
	}
	return nullptr;
}

void AYKHProjectCharacter::AttachWeapon()
{
	if (WeaponItem == nullptr)
	{
		return;
	}

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, false);
	WeaponItem->AttachToComponent(this->GetMesh(), AttachRules, TEXT("WeaponSocket"));
}

void AYKHProjectCharacter::DetachWeapon()
{
	if (WeaponItem == nullptr)
	{
		return;
	}

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, false);
	WeaponItem->DetachFromActor(DetachRules);
	WeaponItem->SetActorScale3D(FVector(2.f, 2.f, 2.f));
	WeaponItem = nullptr;
}
