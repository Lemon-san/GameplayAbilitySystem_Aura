// Copyright Jose Sotomayor


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Aura/Aura.h"
#include "AuraGameplayTags.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();

	bDead = true;
	
}


TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	AuraAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	AuraAbilitySystemComponent->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	//TODO return correct socket based on montage tag (could do a TMAP)

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_RightHand) && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(RightHandTipSocketName);
	}

	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_LeftHand) && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(LeftHandTipSocketName);
	}

	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Tail) && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}

	return FVector();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{

	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}

	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::AddToMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

void AAuraCharacterBase::Dissolve()
{
	UMaterialInstanceDynamic* MeshDynamicMatInst;
	UMaterialInstanceDynamic* WeaponDynamicMatInst;

	if (MeshDissolveMaterialInstance)
	{
		DissolveMaterial(MeshDissolveMaterialInstance, GetMesh(), MeshDynamicMatInst);
		StartMeshDissolveTimeline(MeshDynamicMatInst);
	}
	

	if (Weapon && WeaponDissolveMaterialInstance)
	{
		DissolveMaterial(WeaponDissolveMaterialInstance, Weapon, WeaponDynamicMatInst);
		StartWeaponDissolveTimeline(WeaponDynamicMatInst);
	}	
}

void AAuraCharacterBase::DissolveMaterial(UMaterialInstance* MaterialInstance, USkeletalMeshComponent* SkeletalMeshComponent, UMaterialInstanceDynamic*& DynamicMatInst)
{
	if (IsValid(MaterialInstance))
	{
		DynamicMatInst = UMaterialInstanceDynamic::Create(MaterialInstance, this);
		SkeletalMeshComponent->SetMaterial(0, DynamicMatInst);
	}
}

void AAuraCharacterBase::InitializeAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}



void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> Attributes, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(Attributes);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(Attributes, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}