// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceWarCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/HealthComponent.h"
#include "Singleton/BaseSingleton.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h" 
#include "Perception/AISense_Sight.h"

ASpaceWarCharacter::ASpaceWarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bReplicates = true;
	bCanWeaponManipulation = true;
	SetCanBeDamaged(false);
	NetUpdateFrequency = 35.f;

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

	AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimCameraComponent"));
	AimCamera->SetupAttachment(WeaponMesh);

	RotateArmComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RotateArmComponent"));
	RotateArmComponent->SetupAttachment(FollowCamera);

	SkeletalArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	SkeletalArm->SetupAttachment(RotateArmComponent);

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

	PlayerInputComponent->BindAction("UseJetpack", IE_Released, this, &ASpaceWarCharacter::UseJetpackPressed);
	PlayerInputComponent->BindAction("SuperSprint", IE_Pressed, this, &ASpaceWarCharacter::ToggleUseSuperSprint);

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

	WeaponMesh->AttachToComponent(GetLocalMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponPoint");
	HealthComponent->OnOwnerDead.AddDynamic(this, &ASpaceWarCharacter::CharDead);
	StaminaComponent->OnStaminaUsed.AddDynamic(this, &ASpaceWarCharacter::OnStaminaUsedEvent);
	
	SetActorTickEnabled(false);
	UpdateMeshMaterialByOwnerTeam();
	
	if(IsLocallyControlled() || GetNetMode() == NM_DedicatedServer)
	{
		GetMesh()->SetVisibility(false);
		
		if(GetNetMode() != NM_DedicatedServer)
		{
			auto const GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if(!GI) return;
		
			Server_InitArmor(GI->GetCurrentArmorId());

			for(auto& ByArray : GI->GetWeapons())
			{
				Server_CreateWeapon(ByArray.Key, ByArray.Value);
			}
		}
	}
	else
	{
		SkeletalArm->DestroyComponent();
		RotateArmComponent->DestroyComponent();
	}

	if(GetLocalRole() == ROLE_Authority)
	{
		SetActorTickEnabled(false);
		FTimerHandle TimerHand;
		GetWorld()->GetTimerManager().SetTimer(TimerHand, this, &ASpaceWarCharacter::ReplicateUpPitch, 0.05f, true);
		WeaponManager->CreateThrow("Mine");

		/** 2 ser later after spawn the player can be damage */
		FTimerDelegate UpdateCanBeDamageTimerDelegate;
		UpdateCanBeDamageTimerDelegate.BindLambda([&]() -> void { SetCanBeDamaged(true); });
		FTimerHandle UpdateCanBeDamageHandle;
		GetWorld()->GetTimerManager().SetTimer(UpdateCanBeDamageHandle, UpdateCanBeDamageTimerDelegate, 2.f, false);
	}
	else
	{
		/** Create mini map icon if this not server or owning client */
		if(!IsLocallyControlled()) CreateMinMapIcon("Enemy");
	}
	OnPlayerInitializationComplete.Broadcast();
	StaminaComponent->OnSuperSprintUsed.AddDynamic(this, &ASpaceWarCharacter::ASpaceWarCharacter::OnSuperStaminaUsedEvent);
}

bool ASpaceWarCharacter::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool ParentReturn = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if(ArmorObject) Channel->ReplicateSubobject(ArmorObject, *Bunch, *RepFlags);
	
	return ParentReturn;
}

void ASpaceWarCharacter::Jump()
{
	if(!StaminaComponent->GetSuperSprintSpeed() || !StaminaComponent->IsStaminaUse()) Super::Jump();
}

void ASpaceWarCharacter::Server_InitArmor_Implementation(const FName& ArmorName)
{
	CreateArmor(ArmorName);
}

bool ASpaceWarCharacter::Server_InitArmor_Validate(const FName& ArmorName)
{
	return ArmorDataAsset->CheckId(ArmorName);
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
			GetCharacterMovement()->MaxWalkSpeed = ArmorObject->GetData().MaxStaminaSpeed;

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
			GetCharacterMovement()->MaxWalkSpeed = ArmorObject->GetData().MaxBaseSpeed;
		}
	}
}

void ASpaceWarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Controller)
	{
		FollowCamera->SetWorldRotation(GetController()->GetControlRotation());
		if(IsLocallyControlled())
		{
			RecoilTimeline.TickTimeline(DeltaTime);
		}
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
	DOREPLIFETIME_CONDITION(ASpaceWarCharacter, ArmorObject, COND_OwnerOnly);
}

void ASpaceWarCharacter::CharDead()
{
	if(SkeletalArm) SkeletalArm->SetVisibility(false);
    GetMesh()->SetVisibility(true);
    WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/** Change fisics if this not dedicated server */
	if(GetNetMode() != NM_DedicatedServer && GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->SetSimulatePhysics(true);
	}

	/** Set timer for destroy with out controller */
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() -> void { Destroy(); });
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
	return IsLocallyControlled() || GetNetMode() == NM_DedicatedServer ? SkeletalArm : GetMesh();
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

void ASpaceWarCharacter::UpdateWeaponMesh(UBaseWeaponObject* Weapon)
{
	if(!Weapon) return;
	SyncLoadMesh(Weapon->GetWeaponMesh());
	AimCamera->AttachToComponent(GetWeaponMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SKT_Aim");

	if(Weapon->GetRecoilCurveVector())
	{
		RecoilTimeline.Stop();
		FOnTimelineVector OnTimelineVector;
		OnTimelineVector.BindUFunction(this, "OnUpdateRecoilTimeLine");
		RecoilTimeline.AddInterpVector(Weapon->GetRecoilCurveVector(), OnTimelineVector);
		RecoilTimeline.SetLooping(true);
	}
}

void ASpaceWarCharacter::GetCauserInfo_Implementation(FDamageCauserInfo& DamageCauserInfo)
{
	auto const RangeWeapon = Cast<URangeWeaponObjectBase>(WeaponManager->GetCurrentWeapon());
	if(!RangeWeapon) return;
	DamageCauserInfo.CauserName = RangeWeapon->GetWeaponData().WeaponName;
}

void ASpaceWarCharacter::UseJetpackPressed()
{
	if(!ArmorObject) return;
	if(JetpackComponent->IsAbleToUseJetpack() && ArmorObject->GetData().bCanUseJetPack)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UseJetpackSound, GetActorLocation(),FRotator::ZeroRotator,
            UseJetpackSound->VolumeMultiplier, UseJetpackSound->PitchMultiplier, 0, UseJetpackSound->AttenuationSettings);
		
		Server_UseJetpack();
	}
}

void ASpaceWarCharacter::Server_UseJetpack_Implementation()
{
	if(!ArmorObject->GetData().bCanUseJetPack) return;
	
	FVector Location;
	JetpackComponent->StartUseJetpack(Controller->GetControlRotation().Vector(), Location);
	LaunchCharacter(Location, true, false);
	NetMulticast_PlayUseJetpackSound();
}

void ASpaceWarCharacter::NetMulticast_PlayUseJetpackSound_Implementation()
{
	if(!Controller)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UseJetpackSound, GetActorLocation(),FRotator::ZeroRotator,
			UseJetpackSound->VolumeMultiplier, UseJetpackSound->PitchMultiplier, 0, UseJetpackSound->AttenuationSettings);
	}
}

void ASpaceWarCharacter::OwnerStartUseStamina()
{
	if(!bCanWeaponManipulation || WeaponManager->GetCurrentWeapon()->GetAdditionalUse() || !bMoveForward || StaminaComponent->GetCurrentStaminaValue() <= 0) return;
	
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
	if(!WeaponManager->GetCurrentWeapon()) return;
	
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
	if(!WeaponManager->GetCurrentWeapon()) return;
	
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
	if(bCanWeaponManipulation && WeaponManager->GetCurrentWeapon())
	{
		RecoilTimeline.PlayFromStart();
		WeaponManager->GetCurrentWeapon()->OwnerStartUseWeapon();
	}
}

void ASpaceWarCharacter::StopUseWeapon()
{
	if(WeaponManager->GetCurrentWeapon())
	{
		WeaponManager->GetCurrentWeapon()->OwnerStopUseWeapon();
	}
}

void ASpaceWarCharacter::RefreshAmmo_Implementation()
{
	UpdateAmmo();
}

void ASpaceWarCharacter::UpdateAmmo_Implementation()
{
	auto const Throw = WeaponManager->GetThrow();
	if(Throw)
	{
		Throw->IncrementCurrentAmount();
	}

	for(const auto& ByArray : WeaponManager->GetWeapons())
	{
		auto const RangeWeapon = Cast<URangeWeaponObjectBase>(ByArray.Value);
		if(RangeWeapon)
		{
			RangeWeapon->AddAmmo(RangeWeapon->GetWeaponData().AmmoStatistics.MaxAmmoInStorage / 2);
		}
	}
}

void ASpaceWarCharacter::OnUpdateRecoilTimeLine(const FVector& Vector)
{
	WeaponRecoil = Vector;
}

void ASpaceWarCharacter::UpdateWeaponRecoil()
{
	/** Add Vertical recoil */
	AddControllerPitchInput(-WeaponRecoil.Z * 1.8f);

	/** Add horizontal recoil */
	AddControllerYawInput(-WeaponRecoil.X * 1.8f);
}

void ASpaceWarCharacter::Server_CreateWeapon_Implementation(EWeaponType Type, const FName& Id)
{
#if UE_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("%s"), *Id.ToString()));
#endif
	
	auto const TempWeapon = WeaponManager->CreateWeaponByName(Id, Type);
	if(Type == EWeaponType::FirstWeapon) WeaponManager->SetCurrentWeapon(TempWeapon);
}

void ASpaceWarCharacter::CreateArmor(const FName& Id)
{
	if(GetLocalRole() != ROLE_Authority) return;
	
	ArmorObject = ArmorDataAsset->SyncCreateArmorObject(GetWorld(), ArmorDataAsset->FindData(Id), this);
	GetCharacterMovement()->JumpZVelocity = ArmorObject->GetData().JumpLenght;
	HealthComponent->Init(ArmorObject->GetData().MaxArmor, ArmorObject->GetData().ArmorRegenerationPerSec);
	GetCharacterMovement()->MaxWalkSpeed = ArmorObject->GetData().MaxBaseSpeed;
}

void ASpaceWarCharacter::Server_ReplacementArmor_Implementation(const FName& Id)
{
	if(GetLocalRole() != ROLE_Authority) return;
	
	auto const TempArmor = ArmorObject;
	CreateArmor(Id);
	//TempArmor->ConditionalBeginDestroy();
}

void ASpaceWarCharacter::Server_ReplacementThrow_Implementation(const FName& Id)
{
	auto const TempTrow = WeaponManager->GetThrow();
	WeaponManager->CreateThrow(Id);
}

void ASpaceWarCharacter::ToggleUseSuperSprint()
{
	if(!StaminaComponent->IsCanToggleSuperStamina()) return;
	if(StaminaComponent->IsSuperSprintUsed())
	{
		GetCharacterMovement()->MaxWalkSpeed = ArmorObject->GetData().MaxBaseSpeed;
		Server_StopUseSuperSprint();
		GetLocalMesh()->SetVisibility(true);
	}
	else
	{
		if(bCanWeaponManipulation && !WeaponManager->GetCurrentWeapon()->GetAdditionalUse() && bMoveForward && !GetCharacterMovement()->IsFalling())
		{
			GetCharacterMovement()->MaxWalkSpeed = GetStaminaComponent()->GetSuperSprintSpeed();
			Server_StartUseSuperSprint();
		}
	}
}

void ASpaceWarCharacter::Server_StartUseSuperSprint_Implementation()
{
	if(!StaminaComponent->IsCanToggleSuperStamina()) return;
	
	bCanWeaponManipulation = false;
	GetCharacterMovement()->MaxWalkSpeed = GetStaminaComponent()->GetSuperSprintSpeed();
	StaminaComponent->StartUseSuperSprint();
}

void ASpaceWarCharacter::Server_StopUseSuperSprint_Implementation()
{
	if(!StaminaComponent->IsCanToggleSuperStamina()) return;
	
	GetCharacterMovement()->MaxWalkSpeed = ArmorObject->GetData().MaxBaseSpeed;
	StaminaComponent->StopUseSuperSprint();
}

void ASpaceWarCharacter::OnSuperStaminaUsedEvent(bool bUse)
{
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	if(bUse)
	{
		TimerDel.BindLambda([&]() -> void
		{
			WeaponMesh->AttachToComponent(GetLocalMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SKT_FlagPoint");
            if(IsLocallyControlled()) GetLocalMesh()->SetVisibility(false);
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, WeaponManager->GetCurrentWeapon()->GetSelectTime(), false);
	}
	else
	{
		WeaponMesh->AttachToComponent(GetLocalMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponPoint");
		TimerDel.BindLambda([&]() -> void
		{
			bCanWeaponManipulation = true;
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, WeaponManager->GetCurrentWeapon()->GetSelectTime(), false);
	}
}