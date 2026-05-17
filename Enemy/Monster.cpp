#include "Enemy/Monster.h"
#include "Tables/BATableManager.h"
#include "Tables/MonsterRows.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"

AMonster::AMonster()
{
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterTid != 0)
	{
		InitializeFromTable(MonsterTid);
	}
}

void AMonster::InitializeFromTable(int32 InTid)
{
	Super::InitializeFromTable(InTid);

	UBATableManager* TableManager = UBATableManager::Get(this);
	if (!TableManager) return;

	const FMonsterRows* Row = TableManager->FindMonster(InTid);
	if (!Row) return;

	if (!Row->MeshPath.IsEmpty())
	{
		USkeletalMesh* LoadedMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *Row->MeshPath));
		if (LoadedMesh)
		{
			GetMesh()->SetSkeletalMesh(LoadedMesh);
		}
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	}
}

void AMonster::OnDeath()
{
	Super::OnDeath();
}