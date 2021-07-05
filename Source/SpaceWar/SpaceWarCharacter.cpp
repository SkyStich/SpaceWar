// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceWarCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Singleton/BaseSingleton.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h" 
#include "Perception/AISense_Sight.h"

ASpaceWarCharacter::ASpaceWarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bReplicates = true;
	bCanWeaponManipulation = true;
	NetUpdateFrequency = 25.f;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	RunSpeed = GetCharacterMovement()->MaxWalkSpeed;
	StaminaSpeed = RunSpeed * 1.6f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetupAttachment(RootComponent);

	AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimCameraComponent"));
	AimCamera->SetupAttachment(WeaponMesh);

	SkeletalArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	SkeletalArm->SetupAttachment(FollowCamera);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeaponMesh"));
	WeaponManager = CreateDefaultSubobject<UEquipableWeaponManager>(TEXT("Weapon manager"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	JetpackComponent = CreateDefaultSubobject<UJetpackComponent>(TEXT("JetpackComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliComponent"));
	PerceptionStimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
	PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
}

void ASpaceWarCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Stamina", IE_Pressed, this, &ASpaceWarCharacter::OwnerStartUseStamina);
	PlayerInputComponent->BindAction("Stamina", IE_Released, this, &ASpaceWarCharacter::OwnerStopUseStamina);
	
	PlayerInputComponent->BindAction("AdditionWeapon", IE_Pressed, this, &ASpaceWarCharacter::OwnerStartAdditionalUse);
	PlayerInputComponent->BindAction("AdditionWeapon", IE_Released, this, &ASpaceWarCharacter::OwnerStopAdditionalUse);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceWarCharacter::StartUseWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASpaceWarCharacter::StopUseWeapon);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpaceWarCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceWarCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASpaceWarCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASpaceWarCharacter::LookUpAtRate);
}

void ASpaceWarCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh->AttachToComponent(GetLocalMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponPoint");
	IsLocallyControlled() || GetNetMode() == NM_DedicatedServer ? GetMesh()->SetVisibility(false) : SkeletalArm->DestroyComponent();
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Locally: %d"), IsLocallyControlled()));

	HealthComponent->OnOwnerDead.AddDynamic(this, &ASpaceWarCharacter::CharDead);
	StaminaComponent->OnStaminaUsed.AddDynamic(this, &ASpaceWarCharacter::OnStaminaUsedEvent);
	
	SetActorTickEnabled(false);

	if(GetLocalRole() == ROLE_Authority)
	{
		SetActorTickEnabled(false);
		FTimerHandle TimerHand;
		GetWorld()->GetTimerManager().SetTimer(TimerHand, this, &ASpaceWarCharacter::ReplicateUpPitch, 0.05f, true);
	}
}

void ASpaceWarCharacter::ReplicateUpPitch()
{
	if(Controller)
		LookUpPitch = Controller->GetControlRotation().Vector().Rotation().Pitch;
}

void ASpaceWarCharacter::OnStaminaUsedEvent(bool bState)
{
	if(bState)
	{
		/** return if this not server or owner clinet */
		if(!Controller) return;

		if(GetLocalRole() == ROLE_Authority)
		{
			GetCharacterMovement()->MaxWalkSpeed = StaminaSpeed;
			if(!bCanWeaponManipulation || WeaponManager->GetCurrentWeapon()->GetAdditionalUse())
			{
				StaminaComponent->Server_StopUseStamina();
				return;
			}
		}
		bCanWeaponManipulation = false;
	}
	else
	{
		if(!Controller) return;
		bCanWeaponManipulation = true;
		
		if(GetLocalRole() == ROLE_Authority)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
	}
}

void ASpaceWarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Controller)
	{
		FollowCamera->SetWorldRotation(GetController()->GetControlRotation());
	}
}

void ASpaceWarCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	WeaponManager->OnCurrentWeaponChanged.AddDynamic(this, &ASpaceWarCharacter::UpdateWeaponMesh);
}

void ASpaceWarCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpaceWarCharacter, LookUpPitch);
	DOREPLIFETIME(ASpaceWarCharacter, bCanWeaponManipulation);
}

void ASpaceWarCharacter::CharDead()
{
	if(SkeletalArm) SkeletalArm->SetVisibility(false);
    GetMesh()->SetVisibility(true);
    WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/** Change fisics if this not dedicated server */
	if(GetNetMode() != NM_DedicatedServer)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->SetSimulatePhysics(true);
	}

	/** Set timer for destroy with out controller */
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() -> void
	{
		Destroy();
	});
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDel, 5.f, false);
}

void ASpaceWarCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASpaceWarCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASpaceWarCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		if(Value > 0)
		{
			bMoveForward = true;
		}
		else
		{
			bMoveForward = false;
			if(StaminaComponent->IsStaminaUse())
				OwnerStopUseStamina();
		}
		return;
	}
	bMoveForward = false;
	if(StaminaComponent->IsStaminaUse())
    	OwnerStopUseStamina();
}

void ASpaceWarCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		bMoveForward = false;
		if(StaminaComponent->IsStaminaUse())
			OwnerStopUseStamina();
	}
}

USkeletalMeshComponent* ASpaceWarCharacter::GetLocalMesh() const
{
	return (IsLocallyControlled() || GetNetMode() == NM_DedicatedServer) ? SkeletalArm : GetMesh();
}

void ASpaceWarCharacter::SyncLoadMesh(TAssetPtr<USkeletalMesh> MeshPtr)
{
	if(MeshPtr.IsPending())
	{
		FSoftObjectPath const Ref = MeshPtr.ToSoftObjectPath();
		MeshPtr = Cast<USkeletalMesh>(UBaseSingleton::Get().AssetLoader.LoadSynchronous(Ref));
	}
	WeaponMesh->SetSkeletalMesh(MeshPtr.Get());
}

void ASpaceWarCharacter::UpdateWeaponMesh(URangeWeaponObjectBase* Weapon)
{
	if(!Weapon) return;
	SyncLoadMesh(Weapon->GetWeaponMesh());
	AimCamera->AttachToComponent(GetWeaponMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SKT_Aim");
}

void ASpaceWarCharacter::GetCauserInfo_Implementation(FDamageCauserInfo& DamageCauserInfo)
{
	DamageCauserInfo.CauserName = WeaponManager->GetCurrentWeapon()->GetWeaponData().WeaponName;
}

void ASpaceWarCharacter::UseJetpackPressed()
{
	if(JetpackComponent->IsAbleToUseJetpack())
	{
		Server_UseJetpack();
	}
}

void ASpaceWarCharacter::Server_UseJetpack_Implementation()
{
	FVector Location;
	JetpackComponent->StartUseJetpack(Controller->GetControlRotation().Vector(), Location);
	LaunchCharacter(Location, true, false);
}

void ASpaceWarCharacter::OwnerStartUseStamina()
{
	if(!bCanWeaponManipulation || WeaponManager->GetCurrentWeapon()->GetAdditionalUse() || !bMoveForward) return;
	if(StaminaComponent->GetCurrentStaminaValue() <= 0 || WeaponManager->GetCurrentWeapon()->GetAdditionalUse()) return;
	
	StaminaComponent->Server_StartUseStamina();
}

void ASpaceWarCharacter::OwnerStopUseStamina()
{
	StaminaComponent->Server_StopUseStamina();
}

void ASpaceWarCharacter::Server_StartUseAccessionWeapon_Implementation()
{
	if(WeaponManager->GetWeaponSelect() || WeaponManager->GetCurrentWeapon()->GetAdditionalUse() || !bCanWeaponManipulation) return;
	
	StaminaComponent->StopUseStamina();
	WeaponManager->GetCurrentWeapon()->StartAdditionalUsed();
	SetActorTickEnabled(true);
	StartAiming();
}

void ASpaceWarCharacter::Server_StopUseAccessionWeapon_Implementation()
{
	WeaponManager->GetCurrentWeapon()->StopAdditionalUsed();
	SetActorTickEnabled(false);
	StopAiming();
}

void ASpaceWarCharacter::OwnerStartAdditionalUse()
{
	if(WeaponManager->GetWeaponSelect() && !bCanWeaponManipulation || StaminaComponent->IsStaminaUse()) return;
	
	if(WeaponManager->GetCurrentWeapon()->OwnerStartAdditionalUsed())
	{
		Server_StartUseAccessionWeapon();
		SetActorTickEnabled(true);
		StartAiming();
	}
}

void ASpaceWarCharacter::OwnerStopAdditionalUse()
{
	if(WeaponManager->GetCurrentWeapon()->OwnerStopAdditionalUsed())
	{
		SetActorTickEnabled(false);
		Server_StopUseAccessionWeapon();
		StopAiming();
	}
}

FVector ASpaceWarCharacter::GetCurrentFireTrace() const
{
	return GetWeaponManager()->GetCurrentWeapon()->GetAdditionalUse() ? AimCamera->GetComponentLocation() : FollowCamera->GetComponentLocation();
}

void ASpaceWarCharacter::StartPlayerFirstAid_Implementation(ETeam Team, float const Value)
{
	if(!Controller) return;
	if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(Controller->PlayerState) == Team)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(HealthComponent, &UHealthComponent::FirstAid, Value);
		GetWorld()->GetTimerManager().SetTimer(FirstAidHandle, TimerDel, 1.f, true);
	}
}

void ASpaceWarCharacter::StopPlayerFirstAid_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(FirstAidHandle);
}

void ASpaceWarCharacter::StartUseWeapon()
{
	if(bCanWeaponManipulation)
		WeaponManager->GetCurrentWeapon()->OwnerStartUseWeapon();
}

void ASpaceWarCharacter::StopUseWeapon()
{
	WeaponManager->GetCurrentWeapon()->OwnerStopUseWeapon();
}