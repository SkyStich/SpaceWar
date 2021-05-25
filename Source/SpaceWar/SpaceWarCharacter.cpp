// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceWarCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Singleton/BaseSingleton.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h" 

ASpaceWarCharacter::ASpaceWarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bReplicates = true;

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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->SetupAttachment(RootComponent);

	SkeletalArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	SkeletalArm->SetupAttachment(FollowCamera);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeaponMesh"));
	WeaponManager = CreateDefaultSubobject<UEquipableWeaponManager>(TEXT("Weapon manager"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	JetpackComponent = CreateDefaultSubobject<UJetpackComponent>(TEXT("JetpackComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
}

void ASpaceWarCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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
	IsLocallyControlled() ? GetMesh()->SetVisibility(false) : SkeletalArm->SetVisibility(false);

	HealthComponent->OnOwnerDead.AddDynamic(this, &ASpaceWarCharacter::CharDead);
}

void ASpaceWarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(GetLocalRole() == ROLE_Authority && Controller)
		LookUpPitch = Controller->GetControlRotation().Vector().Rotation().Pitch;
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
}

void ASpaceWarCharacter::CharDead()
{
	SkeletalArm->SetVisibility(false);
    GetMesh()->SetVisibility(true);
    WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    		
	if(GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->SetSimulatePhysics(true);
	}
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
	}
}

void ASpaceWarCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

USkeletalMeshComponent* ASpaceWarCharacter::GetLocalMesh() const
{
	return IsLocallyControlled() ? SkeletalArm : GetMesh();
}

void ASpaceWarCharacter::SyncLoadMesh(TAssetPtr<USkeletalMesh> MeshPtr)
{
	if(MeshPtr.IsPending())
	{
		FSoftObjectPath Ref = MeshPtr.ToSoftObjectPath();
		MeshPtr = Cast<USkeletalMesh>(UBaseSingleton::Get().AssetLoader.LoadSynchronous(Ref));
	}
	WeaponMesh->SetSkeletalMesh(MeshPtr.Get());
}

void ASpaceWarCharacter::UpdateWeaponMesh(URangeWeaponObjectBase* Weapon)
{
	if(!Weapon) return;
	SyncLoadMesh(Weapon->GetWeaponMesh());
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

