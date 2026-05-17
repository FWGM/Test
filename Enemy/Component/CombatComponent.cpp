#include "Component/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "Enemy/EnemyBase.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsHitChecking)
	{
		ProcessHitCheck();
	}
}

void UCombatComponent::ExecuteAttack(UAnimMontage* AttackMontage, float PlayRate)
{
	if (AttackMontage == nullptr)
	{
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->PlayAnimMontage(AttackMontage, PlayRate);
	}
}

void UCombatComponent::StartHitCheck(float InRadius, float InDamage, FName InSocketName)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	bIsHitChecking = true;
	CurrentRadius = InRadius;
	CurrentDamage = InDamage;
	CurrentSocketName = InSocketName;
	
	PrevSocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(CurrentSocketName);
	HitActors.Empty();
	
	SetComponentTickEnabled(true);
}

void UCombatComponent::StartHitCheckDefault()
{
	StartHitCheck(CurrentRadius, CurrentDamage, CurrentSocketName);
}

void UCombatComponent::EndHitCheck()
{
	bIsHitChecking = false;
	SetComponentTickEnabled(false);
}

void UCombatComponent::SetAttackData(float InRadius, float InDamage, FName InSocketName)
{
	CurrentRadius = InRadius;
	CurrentDamage = InDamage;
	CurrentSocketName = InSocketName;
}

void UCombatComponent::TriggerHitStop(float Duration)
{

}

void UCombatComponent::ProcessHitCheck()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	FVector CurrentSocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(CurrentSocketName);
	
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);

	EDrawDebugTrace::Type DebugTrace = bShowDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	/** Sweep Trace: 이전 프레임 소켓 위치에서 현재 소켓 위치까지 구체로 스윕 */
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		this,
		PrevSocketLocation,
		CurrentSocketLocation,
		CurrentRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		DebugTrace,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* Victim = Hit.GetActor();
			if (Victim && !HitActors.Contains(Victim))
			{
				HitActors.Add(Victim);
				ApplyDamage(Victim, Hit);
			}
		}
	}

	PrevSocketLocation = CurrentSocketLocation;
}

void UCombatComponent::SpawnShockwave(FVector Location, float Scale)
{

}

void UCombatComponent::ApplyDamage(AActor* Victim, const FHitResult& HitResult)
{
	if (Victim == nullptr)
	{
		return;
	}

	AActor* OwnerActor = GetOwner();
	AController* Instigator = OwnerActor ? OwnerActor->GetInstigatorController() : nullptr;

	FDamageEvent DamageEvent;
	Victim->TakeDamage(CurrentDamage, DamageEvent, Instigator, OwnerActor);

	// 역경직 (HitStop) 발생
	TriggerHitStop(0.05f); // 0.05초 정도로 짧게

	// 충격파 및 왜곡 발생
	SpawnShockwave(HitResult.ImpactPoint, 1.0f);

	// 넉백 처리
	if (AEnemyBase* EnemyVictim = Cast<AEnemyBase>(Victim))
	{
		EnemyVictim->ApplyKnockback(OwnerActor, 500.0f);
	}

	if (OnHitDetected.IsBound())
	{
		OnHitDetected.Broadcast(Victim, HitResult);
	}
}
