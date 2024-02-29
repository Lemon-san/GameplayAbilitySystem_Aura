//Copyright Jose Sotomayor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
class UNiagaraSystem;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	/*Combat Interface*/
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	/*End of Combat Interface*/

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;

protected:
	
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> Attributes, float Level) const;

	void virtual InitializeAttributes() const;

	void AddCharacterAbilities();

	bool bDead = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandTipSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, CAtegory = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/* Dissolve Effects */

	void Dissolve();

	void DissolveMaterial(UMaterialInstance* MaterialInstance, USkeletalMeshComponent* SkeletalMeshComponent, UMaterialInstanceDynamic*& DynamicMatInst);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartMeshDissolveTimeline(UMaterialInstanceDynamic* DyamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DyamicMaterialInstance);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/* Impact Effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	/* Sound Effects */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects | Sounds")
	TObjectPtr<USoundBase> DeathSound;

private:

	UPROPERTY(EditAnywhere, CAtegory = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, CAtegory = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

public:

	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
};
